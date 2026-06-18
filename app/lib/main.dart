import 'package:flutter/material.dart';
import 'package:smart_wearables_app/connection/connection_page.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart Wearables App',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepOrange),
        useMaterial3: true,
      ),

      home: const ConnectionPage(title: 'Connect your device!'),
    );
  }
}