import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'systemtimechangedetector_platform_interface.dart';

/// An implementation of [SystemTimeChangeDetectorPlatform] that uses method channels.
class MethodChannelTimeChangeDetector extends SystemTimeChangeDetectorPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('systemtimechangedetector');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<void> detectTimeChange(VoidCallback? callback) async {
    try {
      methodChannel.setMethodCallHandler((call) async {
        debugPrint("METHOD_NAME ${call.method} ${call.arguments}");
        if (call.method == 'timeChanged') {
          callback?.call();
        }
      });
      var invoke = await methodChannel.invokeMethod<String>('startListening');
      debugPrint("INVOKE $invoke METHOD: ${methodChannel.name}");
    } on Exception catch (e) {
      debugPrint("Failed to start listening for time changes: '${e}'.");
    }
    return;
  }

  @override
  MethodChannel get mChannel => this.methodChannel;
}
