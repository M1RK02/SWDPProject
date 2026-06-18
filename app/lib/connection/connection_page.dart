import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:smart_wearables_app/connection/stream.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:smart_wearables_app/home_page.dart';

// --- BLE Service and Characteristic UUIDs ---
// These are the specific addresses for the BLE device RN4871 (Microchip) on the board.
Uuid serviceUuid = Uuid.parse("49535343-FE7D-4AE5-8FA9-9FAFD205E455");
Uuid characteristicUuid = Uuid.parse(
  "49535343-1E4D-4BD9-BA61-23C647249616",
); // RX Characteristic
Uuid characteristicUuidTX = Uuid.parse(
  "49535343-8841-43F4-A8D4-ECBE34729BB3",
); // TX Characteristic

// --- 1. Widget Definition ---
class ConnectionPage extends StatefulWidget {
  const ConnectionPage({super.key, required this.title});
  final String title;

  @override
  State<ConnectionPage> createState() => _ConnectionPageState();
}

// --- 2. Widget's State Definition ---
class _ConnectionPageState extends State<ConnectionPage> {
  // A filter to only show BLE devices having "BLE_SW" as name
  final String bleDeviceNameFilter = "BLE_SW";

  final flutterReactiveBle = FlutterReactiveBle();

  late StreamSubscription<DiscoveredDevice> scanStream;
  late Stream<ConnectionStateUpdate> currentConnectionStream;
  late StreamSubscription<ConnectionStateUpdate> connection;

  // The RX (Receive) and TX (Transmit) characteristics of the connected device
  late QualifiedCharacteristic _rxCharacteristic;
  late QualifiedCharacteristic _txCharacteristic;

  List<DiscoveredDevice> foundBleDevices = []; // All found devices
  List<DiscoveredDevice> foundBleDevicesFiltered =
      []; // Only the ones matching the filter

  bool permGranted = false;
  bool scanning = false;
  bool connecting = false;
  bool connected = false;

  MyStream incomingBLEStream = MyStream();

  void refreshScreen() {
    setState(() {});
  }

  // --- Permission Handling ---

  // Shows a dialog if permissions are not granted
  Future<void> _showNoPermissionDialog() async => showDialog<void>(
    context: context,
    barrierDismissible: false,
    builder: (BuildContext context) => AlertDialog(
      title: const Text('Permissions Missing'),
      content: const SingleChildScrollView(
        child: ListBody(
          children: <Widget>[
            Text('You have not granted the required permissions.'),
            Text(
              'Location and Bluetooth permissions are mandatory for BLE to work.',
            ),
          ],
        ),
      ),
      actions: <Widget>[
        TextButton(
          child: const Text('Acknowledge'),
          onPressed: () {
            Navigator.of(context).pop();
          },
        ),
      ],
    ),
  );

  // Asks the user for all required permissions
  void _askPermissions() async {
    Map<Permission, PermissionStatus> statuses = await [
      Permission.bluetoothScan,
      Permission.locationWhenInUse,
      Permission.bluetoothConnect,
    ].request();

    // Check if ALL permissions were granted
    if (statuses[Permission.bluetoothScan] == PermissionStatus.granted &&
        statuses[Permission.bluetoothConnect] == PermissionStatus.granted &&
        statuses[Permission.locationWhenInUse] == PermissionStatus.granted) {
      permGranted = true;
      if (!scanning) {
        _startScan();
      }
    } else {
      permGranted = false;
    }
  }

  // --- Scan Logic ---

  // Stops the BLE scan
  void _stopScan() async {
    await scanStream.cancel();
    scanning = false;
    refreshScreen();
  }

  // Starts the BLE scan
  void _startScan() async {
    if (scanning) {
      _stopScan();
    }

    if (permGranted) {
      foundBleDevices = [];
      foundBleDevicesFiltered = [];
      scanning = true;
      refreshScreen();

      scanStream = flutterReactiveBle
          .scanForDevices(withServices: [/*serviceUuid*/])
          .listen(
            (device) {
              if (foundBleDevices.every((element) => element.id != device.id)) {
                foundBleDevices.add(device);
                if (device.name.contains(bleDeviceNameFilter)) {
                  foundBleDevicesFiltered.add(device);
                }
                refreshScreen();
              }
            },
            onError: (Object error) {
              debugPrint("ERROR during scan: $error \n");
              refreshScreen();
            },
          );

      Future.delayed(const Duration(seconds: 10), () {
        if (scanning) {
          _stopScan();
        }
      });
    } else {
      await _showNoPermissionDialog();
    }
  }

  // --- Connection Logic ---

  void _startConnection(int index) async {
    if (scanning) {
      scanStream.cancel();
      scanning = false;
    }

    if (!connected) {
      setState(() {
        connecting = true;
      });

      // Request MTU (Maximum Transmission Unit):
      final mtu = await flutterReactiveBle.requestMtu(
        deviceId: foundBleDevicesFiltered[index].id,
        mtu: 512,
      );

      currentConnectionStream = flutterReactiveBle.connectToDevice(
        id: foundBleDevicesFiltered[index].id,
        connectionTimeout: const Duration(seconds: 5),
      );

      connection = currentConnectionStream.listen(
        (event) {
          var id = event.deviceId.toString();
          switch (event.connectionState) {
            case DeviceConnectionState.connecting:
              {
                connectingProcedure(id);
                break;
              }
            case DeviceConnectionState.connected:
              {
                connectionProcedure(id, event);
                break;
              }
            case DeviceConnectionState.disconnected:
              {
                disconnectionProcedure(id);
                break;
              }
            default:
          }
          refreshScreen();
        },
        onError: (Object error) {
          connecting = false;
          connected = false;
          ScaffoldMessenger.of(
            context,
          ).showSnackBar(const SnackBar(content: Text("Connection failed!")));
          debugPrint("ERROR during connection: $error \n");
          _startScan();
          refreshScreen();
        },
      );
    }
  }

  void connectingProcedure(String id) {
    connected = false;
    connecting = true;
    debugPrint("Connecting to $id...\n");
  }

  void connectionProcedure(String id, ConnectionStateUpdate event) {
    connected = true;
    connecting = false;
    debugPrint("Connected to $id\n");

    // --- 1. Setup RECEIVE (RX) ---
    _rxCharacteristic = QualifiedCharacteristic(
      serviceId: serviceUuid,
      characteristicId: characteristicUuid,
      deviceId: event.deviceId,
    );

    // --- Packet Buffering Logic ---
    const int fixedPacketLength = 20; // Our custom packet length (in bytes)
    List<int> packetBuffer = []; // Temporary buffer

    flutterReactiveBle
        .subscribeToCharacteristic(_rxCharacteristic)
        .listen(
          (packet) {
            debugPrint("Raw packet: ${packet.length} bytes --> $packet ");

            if ((packetBuffer.length + packet.length) / fixedPacketLength < 1) {
              debugPrint("pacchetto brutto: ${packet.length}");
            } else {
              packetBuffer.addAll(packet);
              int numPacketsReceived = (packetBuffer.length / fixedPacketLength)
                  .floor();

              for (int i = 0; i < numPacketsReceived; i++) {
                List<int> data = packetBuffer.sublist(0, fixedPacketLength);
                packetBuffer.removeRange(0, fixedPacketLength);

                // data[0] == '{' (ASCII 123) AND data[19] == '}' (ASCII 125)
                if (data[0] == 123 && data[fixedPacketLength - 1] == 125) {
                  incomingBLEStream.setNum(data);
                  debugPrint(
                    "Valid packet received (Type: ${String.fromCharCode(data[1])})",
                  );
                } else {
                  debugPrint("Discarding invalid packet: $data");
                }
              }
            }
          },
          onError: (dynamic error) {
            debugPrint("ERROR during RX listen: ${error.toString()}\n");
          },
        );

    // --- 2. Setup TRANSMIT (TX) ---
    _txCharacteristic = QualifiedCharacteristic(
      serviceId: serviceUuid,
      characteristicId: characteristicUuidTX,
      deviceId: event.deviceId,
    );

    incomingBLEStream.controllerSend.stream.listen((event) async {
      flutterReactiveBle.writeCharacteristicWithoutResponse(
        _txCharacteristic,
        value: event,
      );
    });

    // --- 3. Navigation ---
    ScaffoldMessenger.of(
      context,
    ).showSnackBar(const SnackBar(content: Text("Connected!")));

    // Go to the next page (HomePage)
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (context) =>
            HomePage(title: "Sensors Data", stream: incomingBLEStream),
      ),
    ).whenComplete(() => forceDisconnection());
  }

  void disconnectionProcedure(String id) {
    if (connected) {
      ScaffoldMessenger.of(
        context,
      ).showSnackBar(const SnackBar(content: Text("Disconnected!")));
    } else {
      ScaffoldMessenger.of(
        context,
      ).showSnackBar(const SnackBar(content: Text("Not connected!")));
    }
    connected = false;
    connecting = false;
    debugPrint("Disconnected from $id\n");

    Navigator.popUntil(context, (route) => route.isFirst);
  }

  @override
  void initState() {
    super.initState();
    _askPermissions();
  }

  void forceDisconnection() async {
    if (connected) {
      connection.cancel();
      ScaffoldMessenger.of(
        context,
      ).showSnackBar(const SnackBar(content: Text("Disconnected!")));
      _startScan();
      setState(() {
        connected = false;
        connecting = false;
      });
    }
  }

  // --- 4. Building the UI (User Interface) ---
  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        Scaffold(
          appBar: AppBar(
            backgroundColor: Theme.of(context).colorScheme.inversePrimary,
            title: Text(widget.title),
          ),
          body: RefreshIndicator(
            onRefresh: () async {
              return _startScan();
            },
            child: ListView.builder(
              itemCount: foundBleDevicesFiltered.length,
              itemBuilder: (context, index) => Card(
                child: ListTile(
                  dense: true,
                  onTap: () {
                    if (!connecting) {
                      _startConnection(index);
                    }
                  },
                  subtitle: Text(foundBleDevicesFiltered[index].id),
                  title: Text("$index: ${foundBleDevicesFiltered[index].name}"),
                ),
              ),
            ),
          ),
        ),

        // --- Loading Overlay ---
        if (connecting)
          const Opacity(
            opacity: 0.5,
            child: ModalBarrier(dismissible: false, color: Colors.black),
          ),
        if (connecting) const Center(child: CircularProgressIndicator()),
      ],
    );
  }
}
