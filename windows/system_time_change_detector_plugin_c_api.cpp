#include "include/timechangedetector/timechangedetector_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "timechangedetector_plugin.h"

void TimechangedetectorPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  timechangedetector::TimechangedetectorPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
