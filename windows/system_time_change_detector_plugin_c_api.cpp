#include "include/system_time_change_detector/system_time_change_detector_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "system_time_change_detector_plugin.h"

void SystemTimeChangeDetectorPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
    system_time_change_detector::SystemTimeChangeDetectorPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
