//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <system_time_change_detector/system_time_change_detector_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) system_time_change_detector_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "SystemTimeChangeDetectorPlugin");
  system_time_change_detector_plugin_register_with_registrar(system_time_change_detector_registrar);
}
