
import 'dart:ui';

import 'package:flutter/services.dart';

import 'timechangedetector_platform_interface.dart';

class Timechangedetector {
  Future<String?> getPlatformVersion() {
    return TimechangedetectorPlatform.instance.getPlatformVersion();
  }

  Future<dynamic> getSystemTimeChange(VoidCallback? callback) {
    return TimechangedetectorPlatform.instance.detectTimeChange(callback);
  }

  MethodChannel getMethod() {
    return TimechangedetectorPlatform.instance.mChannel;
  }
}
