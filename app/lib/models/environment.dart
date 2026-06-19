import 'package:flutter/material.dart';

enum EnvironmentClass {
  outdoor(1,      'Outdoor',        Color(0xFFFFB300), Icons.wb_sunny),
  indoorNoisy(3,  'Indoor noisy',   Color(0xFFE65100), Icons.volume_up),
  indoorSilent(2, 'Indoor silent',  Color(0xFF1976D2), Icons.home),
  dark(4,         'Full dark',      Color(0xFF37474F), Icons.nightlight_round),
  underground(0,  'Underground',    Color(0xFF6A1B9A), Icons.subway),
  unknown(-1,     'Connecting...',  Color(0xFF9E9E9E), Icons.sensors);

  final int code;
  final String label;
  final Color color;
  final IconData icon;

  const EnvironmentClass(this.code, this.label, this.color, this.icon);

  static EnvironmentClass fromCode(int code) => EnvironmentClass.values
      .firstWhere((e) => e.code == code, orElse: () => unknown);
}