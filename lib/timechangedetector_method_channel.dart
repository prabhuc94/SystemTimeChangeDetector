import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'timechangedetector_platform_interface.dart';

/// An implementation of [TimechangedetectorPlatform] that uses method channels.
class MethodChannelTimechangedetector extends TimechangedetectorPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('timechangedetector');

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
        print("METHOD_NAME ${call.method} ${call.arguments} ${String.fromCharCode(int.tryParse(call.arguments.toString()) ?? 0 )}");
        if (call.method == 'timeChanged') {
          callback?.call();
        }
      });
      var invoke = await methodChannel.invokeMethod<String>('startListening');
      print("INVOKE $invoke METHOD: ${methodChannel.name}");
    } on Exception catch (e) {
      print("Failed to start listening for time changes: '${e}'.");
    }
    return;
  }

  @override
  MethodChannel get mChannel => this.methodChannel;
}
