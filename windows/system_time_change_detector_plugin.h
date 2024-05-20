#ifndef FLUTTER_PLUGIN_TIMECHANGEDETECTOR_PLUGIN_H_
#define FLUTTER_PLUGIN_TIMECHANGEDETECTOR_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <sstream>
#include <memory>

namespace timechangedetector {

class TimechangedetectorPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  TimechangedetectorPlugin();

  virtual ~TimechangedetectorPlugin();

  // Disallow copy and assign.
  TimechangedetectorPlugin(const TimechangedetectorPlugin&) = delete;
  TimechangedetectorPlugin& operator=(const TimechangedetectorPlugin&) = delete;

    // Called when a method is called on this plugin's channel from Dart.
   void HandleMethodCall(
            const flutter::MethodCall<flutter::EncodableValue> &method_call,
            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result,
            flutter::PluginRegistrarWindows* registrar);
};

}  // namespace timechangedetector

#endif  // FLUTTER_PLUGIN_TIMECHANGEDETECTOR_PLUGIN_H_
