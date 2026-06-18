import 'dart:async';
import 'package:flutter/material.dart';
import 'package:smart_wearables_app/connection/stream.dart';
import 'package:smart_wearables_app/models/lumos_packet.dart';
import 'package:smart_wearables_app/models/environment.dart';
import 'package:smart_wearables_app/services/packet_parser.dart';

class LumosPage extends StatefulWidget {
  const LumosPage({super.key, required this.stream});
  final MyStream stream;

  @override
  State<LumosPage> createState() => _LumosPageState();
}

class _LumosPageState extends State<LumosPage> {
  late StreamSubscription _sub;
  LumosPacket? _packet;
  bool _showTelemetry = false;

  // Spectral bar colors mapped to approximate wavelengths
  static const _barColors = [
    Color(0xFF8B00FF), // F1 415 nm — violet
    Color(0xFF4169E1), // F2 445 nm — blue
    Color(0xFF1E90FF), // F3 480 nm — dodger blue
    Color(0xFF00CED1), // F4 515 nm — cyan
    Color(0xFF32CD32), // F5 555 nm — lime green
    Color(0xFFFFD700), // F6 590 nm — gold
    Color(0xFFFF6347), // F7 630 nm — tomato
    Color(0xFFDC143C), // F8 680 nm — crimson
    Color(0xFF9E9E9E), // CLEAR — grey
    Color(0xFF4A0010), // NIR  — dark maroon
  ];

  @override
  void initState() {
    super.initState();
    _sub = widget.stream.controller.stream.listen((data) {
      final p = PacketParser.parse(data as List<int>);
      if (p != null) setState(() => _packet = p);
    });
  }

  @override
  void dispose() {
    _sub.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final env = _packet != null
        ? EnvironmentClass.fromCode(_packet!.classLabel)
        : EnvironmentClass.unknown;

    return Scaffold(
      appBar: AppBar(
        title: Text(_showTelemetry ? 'Spectral data' : 'Environment'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        actions: [
          IconButton(
            icon: Icon(_showTelemetry ? Icons.home_outlined : Icons.bar_chart),
            tooltip: _showTelemetry ? 'Show classification' : 'Show spectral data',
            onPressed: () => setState(() => _showTelemetry = !_showTelemetry),
          ),
        ],
      ),
      body: _packet == null
          ? const _WaitingView()
          : AnimatedSwitcher(
              duration: const Duration(milliseconds: 300),
              child: _showTelemetry
                  ? _TelemetryView(
                      key: const ValueKey('telemetry'),
                      packet: _packet!,
                      barColors: _barColors,
                    )
                  : _ClassificationView(
                      key: const ValueKey('classification'),
                      env: env,
                    ),
            ),
    );
  }
}

// ─── Waiting ────────────────────────────────────────────────────────────────

class _WaitingView extends StatelessWidget {
  const _WaitingView();

  @override
  Widget build(BuildContext context) {
    return const Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          CircularProgressIndicator(),
          SizedBox(height: 20),
          Text('Waiting for sensor data...'),
        ],
      ),
    );
  }
}

// ─── Classification ──────────────────────────────────────────────────────────

class _ClassificationView extends StatelessWidget {
  const _ClassificationView({super.key, required this.env});
  final EnvironmentClass env;

  @override
  Widget build(BuildContext context) {
    return Container(
      width: double.infinity,
      color: env.color.withOpacity(0.12),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(env.icon, size: 120, color: env.color),
          const SizedBox(height: 32),
          Text(
            env.label,
            style: TextStyle(
              fontSize: 36,
              fontWeight: FontWeight.bold,
              color: env.color,
              letterSpacing: 1.2,
            ),
          ),
          const SizedBox(height: 16),
          Text(
            'tap  ≡  for spectral data',
            style: TextStyle(fontSize: 13, color: Colors.grey[500]),
          ),
        ],
      ),
    );
  }
}

// ─── Telemetry ───────────────────────────────────────────────────────────────

class _TelemetryView extends StatelessWidget {
  const _TelemetryView({
    super.key,
    required this.packet,
    required this.barColors,
  });
  final LumosPacket packet;
  final List<Color> barColors;

  @override
  Widget build(BuildContext context) {
    final ch = packet.channels;
    final names = packet.channelNames;
    final maxVal = ch.reduce((a, b) => a > b ? a : b).toDouble();

    return Padding(
      padding: const EdgeInsets.fromLTRB(16, 24, 16, 16),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text('Live spectral channels',
              style: Theme.of(context).textTheme.titleMedium),
          const SizedBox(height: 6),
          Text(
            'Class: ${packet.classLabel}  ·  '
            'CLEAR: ${ch[8]}  ·  NIR: ${ch[9]}',
            style: TextStyle(fontSize: 13, color: Colors.grey[600]),
          ),
          const SizedBox(height: 24),
          SizedBox(
            height: 280,
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.end,
              children: List.generate(10, (i) {
                final frac = maxVal > 0 ? ch[i] / maxVal : 0.0;
                final label = ch[i] >= 10000
                    ? '${(ch[i] / 1000).toStringAsFixed(1)}k'
                    : '${ch[i]}';
                return Expanded(
                  child: Padding(
                    padding: const EdgeInsets.symmetric(horizontal: 3),
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.end,
                      children: [
                        Text(label,
                            style: const TextStyle(fontSize: 8),
                            textAlign: TextAlign.center),
                        const SizedBox(height: 2),
                        AnimatedContainer(
                          duration: const Duration(milliseconds: 250),
                          height: frac * 220,
                          decoration: BoxDecoration(
                            color: barColors[i],
                            borderRadius: const BorderRadius.vertical(
                              top: Radius.circular(4),
                            ),
                          ),
                        ),
                        const SizedBox(height: 4),
                        Text(names[i],
                            style: const TextStyle(fontSize: 9),
                            textAlign: TextAlign.center),
                      ],
                    ),
                  ),
                );
              }),
            ),
          ),
        ],
      ),
    );
  }
}