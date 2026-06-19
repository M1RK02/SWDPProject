import 'dart:async';
import 'dart:io' show Platform;
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:smart_wearables_app/connection/stream.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:smart_wearables_app/lumos_page.dart';

// --- BLE Service and Characteristic UUIDs (RN4871 Microchip Transparent UART) ---
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
  // A filter to only show BLE devices having "BLE_A8" as name
  final String bleDeviceNameFilter = "BLE_A8";

  final flutterReactiveBle = FlutterReactiveBle();

  late StreamSubscription<DiscoveredDevice> scanStream;
  late Stream<ConnectionStateUpdate> currentConnectionStream;
  late StreamSubscription<ConnectionStateUpdate> connection;
  StreamSubscription<List<int>>? _rxSubscription;

  // The RX (Receive) and TX (Transmit) characteristics of the connected device
  late QualifiedCharacteristic _rxCharacteristic;
  late QualifiedCharacteristic _txCharacteristic;

  List<DiscoveredDevice> foundBleDevices = []; // All found devices
  List<DiscoveredDevice> foundBleDevicesFiltered = []; // Filtered ones

  bool permGranted = false;
  bool scanning = false;
  bool connecting = false;
  bool connected = false;

  MyStream incomingBLEStream = MyStream();

  // --- Packet framing constants & buffer ---
  static const int _kPacketLen = 24;
  static const int _kStart = 0x7B; // '{'
  static const int _kEnd = 0x7D; // '}'
  final List<int> _packetBuffer = [];

  // --- Demo mode ---
  Timer? _demoTimer;
  MyStream? _demoStream;

  void refreshScreen() {
    if (mounted) setState(() {});
  }

  // --- Permission Handling ---

  Future<void> _showNoPermissionDialog() async => showDialog<void>(
    context: context,
    barrierDismissible: false,
    builder: (BuildContext context) => AlertDialog(
      title: const Text('Permissions Missing'),
      content: const SingleChildScrollView(
        child: ListBody(
          children: <Widget>[
            Text('You have not granted the required permissions.'),
            Text('Bluetooth permission is mandatory for BLE to work.'),
          ],
        ),
      ),
      actions: <Widget>[
        TextButton(
          child: const Text('Acknowledge'),
          onPressed: () => Navigator.of(context).pop(),
        ),
      ],
    ),
  );

  // Asks for the right permissions per platform.
  // iOS needs only Bluetooth. Android also needs Location for BLE scanning.
  void _askPermissions() async {
    if (Platform.isIOS) {
      // iOS: flutter_reactive_ble handles the Bluetooth permission itself
      // when scanForDevices is called. permission_handler is not needed.
      setState(() => permGranted = true);
      if (!scanning) _startScan();
      return;
    }

    // Android: explicitly request scan, connect, and location
    final statuses = await [
      Permission.bluetoothScan,
      Permission.bluetoothConnect,
      Permission.locationWhenInUse,
    ].request();

    debugPrint("Permission statuses: $statuses");
    final ok = statuses.values.every((s) => s.isGranted);
    setState(() => permGranted = ok);

    if (ok && !scanning) _startScan();
  }

  // --- Scan Logic ---

  void _stopScan() async {
    await scanStream.cancel();
    scanning = false;
    refreshScreen();
  }

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
          .scanForDevices(withServices: [])
          .listen(
            (device) {
              debugPrint(
                "Found: ${device.name.isEmpty ? '(unnamed)' : device.name}",
              );
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

      currentConnectionStream = flutterReactiveBle.connectToDevice(
        id: foundBleDevicesFiltered[index].id,
        connectionTimeout: const Duration(seconds: 5),
      );

      connection = currentConnectionStream.listen(
        (event) {
          var id = event.deviceId.toString();
          switch (event.connectionState) {
            case DeviceConnectionState.connecting:
              connectingProcedure(id);
            case DeviceConnectionState.connected:
              connectionProcedure(id, event);
            case DeviceConnectionState.disconnected:
              disconnectionProcedure(id);
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

    // Fresh buffer for this connection session
    _packetBuffer.clear();

    // --- 1. Setup RECEIVE (RX) ---
    _rxCharacteristic = QualifiedCharacteristic(
      serviceId: serviceUuid,
      characteristicId: characteristicUuid,
      deviceId: event.deviceId,
    );

    // Negotiate a larger MTU now that we're connected (no-op on iOS, helps Android)
    flutterReactiveBle
        .requestMtu(deviceId: event.deviceId, mtu: 512)
        .then((mtu) => debugPrint("MTU negotiated: $mtu"))
        .catchError((e) => debugPrint("MTU request ignored: $e"));

    // --- Packet Buffering Logic (robust, self-resyncing) ---
    _rxSubscription = flutterReactiveBle
        .subscribeToCharacteristic(_rxCharacteristic)
        .listen(
          (chunk) {
            _packetBuffer.addAll(chunk);

            // Drain every complete, correctly-framed packet in the buffer
            while (_packetBuffer.length >= _kPacketLen) {
              // Resync: discard leading bytes until the buffer starts with '{'
              if (_packetBuffer[0] != _kStart) {
                _packetBuffer.removeAt(0);
                continue;
              }
              // Start marker present and we have a full packet's worth of bytes
              if (_packetBuffer[_kPacketLen - 1] == _kEnd) {
                final data = _packetBuffer.sublist(0, _kPacketLen);
                _packetBuffer.removeRange(0, _kPacketLen);
                incomingBLEStream.setNum(data);
                debugPrint(
                  "Valid packet (type: ${String.fromCharCode(data[1])}, class: ${data[2]})",
                );
              } else {
                // False start: '{' wasn't a real header, drop it and resync
                _packetBuffer.removeAt(0);
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
        value: event as List<int>,
      );
    });

    // --- 3. Navigation ---
    ScaffoldMessenger.of(
      context,
    ).showSnackBar(const SnackBar(content: Text("Connected!")));

    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (context) => LumosPage(stream: incomingBLEStream),
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
    _packetBuffer.clear();
    debugPrint("Disconnected from $id\n");

    Navigator.popUntil(context, (route) => route.isFirst);
  }

  void forceDisconnection() async {
    if (connected) {
      await _rxSubscription?.cancel();
      _rxSubscription = null;
      connection.cancel();
      _packetBuffer.clear();
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

  // --- Demo Mode (no hardware) ---
  void _startDemoMode() {
    _demoStream = MyStream();

    Navigator.push(
      context,
      MaterialPageRoute(builder: (context) => LumosPage(stream: _demoStream!)),
    ).whenComplete(() {
      _demoTimer?.cancel();
      _demoTimer = null;
    });

    int tick = 0;
    _demoTimer = Timer.periodic(const Duration(seconds: 2), (timer) {
      if (!mounted) {
        timer.cancel();
        return;
      }
      final label = tick % 5;
      final base = [3000, 8000, 30000, 500, 12000][label];
      final pkt = List<int>.filled(24, 0);
      pkt[0] = _kStart;
      pkt[1] = 0x4C; // 'L'
      pkt[2] = label;
      final channels = [
        (base * 0.3).toInt(),
        (base * 0.5).toInt(),
        (base * 0.7).toInt(),
        (base * 0.9).toInt(),
        (base * 1.0).toInt(),
        (base * 0.8).toInt(),
        (base * 0.6).toInt(),
        (base * 0.4).toInt(),
        base,
        (base * 0.2).toInt(),
      ];
      for (int i = 0; i < 10; i++) {
        final v = channels[i].clamp(0, 65535);
        pkt[3 + i * 2] = v & 0xFF;
        pkt[3 + i * 2 + 1] = (v >> 8) & 0xFF;
      }
      pkt[23] = _kEnd;
      _demoStream!.setNum(pkt);
      tick++;
    });
  }

  @override
  void initState() {
    super.initState();
    _askPermissions();
  }

  @override
  void dispose() {
    _demoTimer?.cancel();
    _rxSubscription?.cancel();
    super.dispose();
  }

  // --- 4. Building the UI ---
  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        Scaffold(
          appBar: AppBar(
            backgroundColor: Theme.of(context).colorScheme.inversePrimary,
            title: Text(widget.title),
          ),
          body: Column(
            children: [
              Expanded(
                child: RefreshIndicator(
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
                        title: Text(
                          "$index: ${foundBleDevicesFiltered[index].name}",
                        ),
                      ),
                    ),
                  ),
                ),
              ),
              Padding(
                padding: const EdgeInsets.all(16),
                child: OutlinedButton.icon(
                  onPressed: _startDemoMode,
                  icon: const Icon(Icons.play_circle_outline),
                  label: const Text('Demo mode (no hardware)'),
                ),
              ),
            ],
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
