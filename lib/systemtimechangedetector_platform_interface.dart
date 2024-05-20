import 'dart:ui';
import 'package:flutter/src/services/platform_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'systemtimechangedetector_method_channel.dart';

abstract class SystemTimeChangeDetectorPlatform extends PlatformInterface {
  /// Constructs a TimechangedetectorPlatform.
  SystemTimeChangeDetectorPlatform() : super(token: _token);

  static final Object _token = Object();

  static SystemTimeChangeDetectorPlatform _instance = MethodChannelTimeChangeDetector();

  /// The default instance of [SystemTimeChangeDetectorPlatform] to use.
  ///
  /// Defaults to [MethodChannelTimeChangeDetector].
  static SystemTimeChangeDetectorPlatform get instance => _instance;

  MethodChannel get mChannel => throw UnimplementedError('methodChannel has not been found.');

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [SystemTimeChangeDetectorPlatform] when
  /// they register themselves.
  static set instance(SystemTimeChangeDetectorPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<void> detectTimeChange(VoidCallback? callback) {
    throw UnimplementedError('detectTimeChange() has not been implemented.');
  }
}
