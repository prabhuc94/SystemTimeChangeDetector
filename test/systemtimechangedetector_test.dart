import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:system_time_change_detector/timechangedetector.dart';
import 'package:system_time_change_detector/timechangedetector_platform_interface.dart';
import 'package:system_time_change_detector/timechangedetector_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockTimechangedetectorPlatform
    with MockPlatformInterfaceMixin
    implements TimechangedetectorPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');

  @override
  Future<dynamic> detectTimeChange(VoidCallback) => Future.value("");

  @override
  MethodChannel get mChannel => throw UnimplementedError();
}

void main() {
  final TimechangedetectorPlatform initialPlatform = TimechangedetectorPlatform.instance;

  test('$MethodChannelTimechangedetector is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelTimechangedetector>());
  });

  test('getPlatformVersion', () async {
    Timechangedetector timechangedetectorPlugin = Timechangedetector();
    MockTimechangedetectorPlatform fakePlatform = MockTimechangedetectorPlatform();
    TimechangedetectorPlatform.instance = fakePlatform;

    expect(await timechangedetectorPlugin.getPlatformVersion(), '42');
  });
}
