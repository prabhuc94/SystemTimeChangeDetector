#include "include/system_time_change_detector/system_time_change_detector_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <sys/inotify.h>
#include <unistd.h>

#include "system_time_change_detector_plugin_private.h"

#define SYSTEM_TIME_CHANGE_DETECTOR_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), system_time_change_detector_plugin_get_type(), \
                              SystemTimeChangeDetectorPlugin))

struct _SystemTimeChangeDetectorPlugin {
  GObject parent_instance;
  FlMethodChannel* channel;
};

G_DEFINE_TYPE(SystemTimeChangeDetectorPlugin, system_time_change_detector_plugin, g_object_get_type())

static SystemTimeChangeDetectorPlugin* g_plugin_instance = nullptr;
static FlMethodCall* method_caller = nullptr;
static FlMethodChannel* method_channel = nullptr;
// Function prototypes
static void* watch_time_changes(void* arg);
static void time_changed();
FlMethodResponse* get_platform_version();

// Called when a method call is received from Flutter.
static void system_time_change_detector_plugin_handle_method_call(
    SystemTimeChangeDetectorPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;
    method_caller = method_call;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    response = get_platform_version();
  } else if (strcmp(method, "startListening") == 0) {
      std::thread(watch_time_changes, nullptr).detach(); // Start listening for time changes in a new thread
      response = FL_METHOD_RESPONSE(fl_method_success_response_new(fl_value_new_string("startListening invoked"))); // Notify Flutter that the method call was successful
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

FlMethodResponse* get_platform_version() {
  struct utsname uname_data = {};
  uname(&uname_data);
  g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
  g_autoptr(FlValue) result = fl_value_new_string(version);
  return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
}

static void system_time_change_detector_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(system_time_change_detector_plugin_parent_class)->dispose(object);
}

static void system_time_change_detector_plugin_class_init(SystemTimeChangeDetectorPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = system_time_change_detector_plugin_dispose;
}

static void system_time_change_detector_plugin_init(SystemTimeChangeDetectorPlugin* self) {
    g_plugin_instance = self;
}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  SystemTimeChangeDetectorPlugin* plugin = SYSTEM_TIME_CHANGE_DETECTOR_PLUGIN(user_data);
  system_time_change_detector_plugin_handle_method_call(plugin, method_call);
}

// Plugin registration
void system_time_change_detector_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  SystemTimeChangeDetectorPlugin* plugin = SYSTEM_TIME_CHANGE_DETECTOR_PLUGIN(
      g_object_new(system_time_change_detector_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel = fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "systemtimechangedetector",
                            FL_METHOD_CODEC(codec));
  method_channel = channel;
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}

// CUSTOM CODE TO DETECT SYSTEM TIME / ZONE CHANGES

// Function to handle the time changed event
static void time_changed() {
    std::cout << "System time or timezone has changed!" << std::endl;
    // Notify Flutter of the time change
    g_autoptr(FlValue) result = fl_value_new_string("Time has changed");
    if (FL_IS_METHOD_CHANNEL(method_channel)) {
        // Call fl_method_channel_invoke_method with the correct arguments
        fl_method_channel_invoke_method(method_channel, "timeChanged", result, nullptr, nullptr, nullptr);
    } else {
        // Handle the case where the channel object is not of the expected type
        g_printerr("Error: Invalid method channel object\n");
    }
}

// Function to watch for time changes using inotify
static void* watch_time_changes(void* arg) {
    int inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        std::cerr << "Failed to initialize inotify!" << std::endl;
        return nullptr;
    }

    int watch_desc_localtime = inotify_add_watch(inotify_fd, "/etc", IN_MODIFY);
    int watch_desc_timezone = inotify_add_watch(inotify_fd, "/etc", IN_MODIFY);

    if (watch_desc_localtime < 0 || watch_desc_timezone < 0) {
        std::cerr << "Failed to add inotify watch!" << std::endl;
        close(inotify_fd);
        return nullptr;
    }

    const size_t event_size = sizeof(struct inotify_event);
    const size_t buf_len = 1024 * (event_size + 16);
    char buffer[buf_len];

    std::cout << "Watching for time changes..." << std::endl;

    while (true) {
        int length = read(inotify_fd, buffer, buf_len);
        if (length < 0) {
            std::cerr << "Error reading from inotify file descriptor!" << std::endl;
            break;
        }

        int i = 0;
        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            std::cout << "Watch descriptor: " << event->wd << ", Cookie: " << event->cookie << ", Event mask: " << event->mask;
            if (event->len > 0) {
                std::cout << ", Name: " << event->name;
            }
            std::cout << std::endl;

            if (event->len > 0 && (event->mask & IN_MODIFY)) {
                std::cout << "File modified: " << event->name << std::endl;
                time_changed();
            }
            i += event_size + event->len;
        }
    }

    close(inotify_fd);
    return nullptr;
}
