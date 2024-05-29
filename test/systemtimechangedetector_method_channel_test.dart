import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:system_time_change_detector/system_time_change_detector_method_channel.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  MethodChannelTimeChangeDetector platform = MethodChannelTimeChangeDetector();
  const MethodChannel channel = MethodChannel('systemtimechangedetector');

  setUp(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(
      channel,
      (MethodCall methodCall) async {
        return '42';
      },
    );
  });

  tearDown(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(channel, null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
