

import 'package:flutter/services.dart';

import 'system_time_change_detector_platform_interface.dart';

class SystemTimeChangeDetector {
  Future<String?> getPlatformVersion() {
    return SystemTimeChangeDetectorPlatform.instance.getPlatformVersion();
  }

  Future<dynamic> getSystemTimeChange(VoidCallback? callback) {
    return SystemTimeChangeDetectorPlatform.instance.detectTimeChange(callback);
  }

  MethodChannel getMethod() {
    return SystemTimeChangeDetectorPlatform.instance.mChannel;
  }
}
