import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:system_time_change_detector/system_time_change_detector.dart';
import 'package:system_time_change_detector/system_time_change_detector_platform_interface.dart';
import 'package:system_time_change_detector/system_time_change_detector_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockTimechangedetectorPlatform
    with MockPlatformInterfaceMixin
    implements SystemTimeChangeDetectorPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');

  @override
  Future<dynamic> detectTimeChange(VoidCallback) => Future.value("");

  @override
  MethodChannel get mChannel => throw UnimplementedError();
}

void main() {
  final SystemTimeChangeDetectorPlatform initialPlatform = SystemTimeChangeDetectorPlatform.instance;

  test('$MethodChannelTimeChangeDetector is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelTimeChangeDetector>());
  });

  test('getPlatformVersion', () async {
    SystemTimeChangeDetector timechangedetectorPlugin = SystemTimeChangeDetector();
    MockTimechangedetectorPlatform fakePlatform = MockTimechangedetectorPlatform();
    SystemTimeChangeDetectorPlatform.instance = fakePlatform;

    expect(await timechangedetectorPlugin.getPlatformVersion(), '42');
  });
}
