//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <timechangedetector/timechangedetector_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) timechangedetector_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "TimechangedetectorPlugin");
  timechangedetector_plugin_register_with_registrar(timechangedetector_registrar);
}
