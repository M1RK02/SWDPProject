import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/material.dart';
import 'package:smart_wearables_app/connection/stream.dart';
import 'package:syncfusion_flutter_charts/charts.dart';
import 'package:smart_wearables_app/utils/sensor_utils.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key, required this.title, required this.stream});
  final String title;
  final MyStream stream;

  @override
  State<HomePage> createState() => _HomePageState();
}

class ChartData {
  ChartData(this.x, this.y);
  final int x;
  final double y;
}

class _HomePageState extends State<HomePage> {
  late StreamSubscription _dataSubscription;

  List<ChartData> xData = [];
  List<ChartData> yData = [];
  List<ChartData> zData = [];

  ChartSeriesController? _xSeriesController;
  ChartSeriesController? _ySeriesController;
  ChartSeriesController? _zSeriesController;

  int xCounter = 0;
  String dataType = 'N/A';
  final int maxDataPoints = 50;
  final double sensitivity = 2.0 / 32767.0;

  @override
  void initState() {
    super.initState();
    _dataSubscription = widget.stream.controller.stream.listen((data) {
      _parsePacket(data);
    });
  }

  void _parsePacket(List<int> packet) {
    String type = String.fromCharCode(packet[1]);

    var byteData = Uint8List.fromList(packet.sublist(2)).buffer.asByteData();

    int rawX = byteData.getInt16(0, Endian.little);
    int rawY = byteData.getInt16(2, Endian.little);
    int rawZ = byteData.getInt16(4, Endian.little);

    double gX = rawX * sensitivity;
    double gY = rawY * sensitivity;
    double gZ = rawZ * sensitivity;

    if (dataType != type) {
      setState(() {
        dataType = type;
      });
    }

    xData.add(ChartData(xCounter, gX));
    yData.add(ChartData(xCounter, gY));
    zData.add(ChartData(xCounter, gZ));
    xCounter++;

    bool isListFull = xData.length > maxDataPoints;
    if (isListFull) {
      xData.removeAt(0);
      yData.removeAt(0);
      zData.removeAt(0);
    }
    
    _xSeriesController?.updateDataSource(
      addedDataIndexes: <int>[xData.length - 1], 
      removedDataIndexes: isListFull ? <int>[0] : null, 
    );
    _ySeriesController?.updateDataSource(
      addedDataIndexes: <int>[yData.length - 1],
      removedDataIndexes: isListFull ? <int>[0] : null,
    );
    _zSeriesController?.updateDataSource(
      addedDataIndexes: <int>[zData.length - 1],
      removedDataIndexes: isListFull ? <int>[0] : null,
    );

    debugPrint(
        "Parsed: Type=$dataType, X=${gX.toStringAsFixed(2)}g, Y=${gY.toStringAsFixed(2)}g, Z=${gZ.toStringAsFixed(2)}g");
  }

  @override
  void dispose() {
    _dataSubscription.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: ListView(
        children: <Widget>[
          SizedBox(height: 10),
          Center(
            child: Text(
              'Sensor Type: ${getSensorNameFromType(dataType)}',
              style: const TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
            ),
          ),
          SizedBox(height: 20),
          
          _buildChart(
            "X-Axis",
            xData,
            Colors.red,
            (controller) => _xSeriesController = controller, 
          ),
          _buildChart(
            "Y-Axis",
            yData,
            Colors.green,
            (controller) => _ySeriesController = controller, 
          ),
          _buildChart(
            "Z-Axis",
            zData,
            Colors.blue,
            (controller) => _zSeriesController = controller, 
          ),
        ],
      ),
    );
  }


  Widget _buildChart(String title, List<ChartData> data, Color color,
      void Function(ChartSeriesController) onControllerCreated) { 
    return Container(
      height: 250,
      padding: EdgeInsets.all(10),
      child: Column(
        children: [
          Text(title, style: TextStyle(fontWeight: FontWeight.bold)),
          Expanded(
            child: SfCartesianChart(
              primaryXAxis: NumericAxis(

                autoScrollingMode: AutoScrollingMode.end,
                autoScrollingDelta: maxDataPoints,

                isVisible: false,
              ),
              primaryYAxis: NumericAxis(
                minimum: -2,
                maximum: 2,
                labelFormat: '{value} g',
              ),
              series: <LineSeries<ChartData, int>>[
                LineSeries<ChartData, int>(

                  onRendererCreated: onControllerCreated,

                  dataSource: data,
                  xValueMapper: (ChartData d, _) => d.x,
                  yValueMapper: (ChartData d, _) => d.y,
                  color: color,
                  animationDuration: 0,
                )
              ],
            ),
          ),
        ],
      ),
    );
  }
}