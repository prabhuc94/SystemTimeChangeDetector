import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:system_time_change_detector/timechangedetector.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = '...';
  String _timeChange = '';
  final _timechangedetectorPlugin = Timechangedetector();

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      platformVersion =
          await _timechangedetectorPlugin.getPlatformVersion() ?? 'Unknown platform version';
      _timechangedetectorPlugin.getMethod().setMethodCallHandler((call) async => print("METHOD_HANDLER ${call.toString()}"));
      await _timechangedetectorPlugin.getSystemTimeChange(() {
        print("TIME CHANGING");
        _timeChange = DateTime.now().toIso8601String();
        setState(() {

        });
      });
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    } catch (e) {
      platformVersion = '$e';
      print("Error $e");
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Text('Running on: $_platformVersion\n'),
              SizedBox(height: 30),
              Text('System Time changed at: $_timeChange'),
            ],
          ),
        ),
      ),
    );
  }
}
