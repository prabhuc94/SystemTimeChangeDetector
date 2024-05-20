import 'dart:ui';

import 'package:flutter/src/services/platform_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'timechangedetector_method_channel.dart';

abstract class TimechangedetectorPlatform extends PlatformInterface {
  /// Constructs a TimechangedetectorPlatform.
  TimechangedetectorPlatform() : super(token: _token);

  static final Object _token = Object();

  static TimechangedetectorPlatform _instance = MethodChannelTimechangedetector();

  /// The default instance of [TimechangedetectorPlatform] to use.
  ///
  /// Defaults to [MethodChannelTimechangedetector].
  static TimechangedetectorPlatform get instance => _instance;

  MethodChannel get mChannel => throw UnimplementedError('methodChannel has not been found.');

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [TimechangedetectorPlatform] when
  /// they register themselves.
  static set instance(TimechangedetectorPlatform instance) {
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
