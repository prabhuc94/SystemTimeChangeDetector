#include "include/systemtimechangedetector/system_time_change_detector_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#include systemtimechangedetector_plugin_private.h

#define TIMECHANGEDETECTOR_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), timechangedetector_plugin_get_type(), \
                              SystemTimeChangeDetectorPlugin))

struct _SystemTimeChangeDetectorPlugin {
  GObject parent_instance;
  FlMethodChannel* channel;
};

G_DEFINE_TYPE(SystemTimeChangeDetectorPlugin, timechangedetector_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void timechangedetector_plugin_handle_method_call(
    SystemTimeChangeDetectorPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    response = get_platform_version();
  } else if (strcmp(method, "startListening") == 0) {
      g_thread_new(NULL, (GThreadFunc)setupTimeChangeListener, NULL); // Start listening for time changes in a new thread
      fl_method_response_success(response, fl_value_new_string("startListening invoked")); // Notify Flutter that the method call was successful
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void time_changed() {
    std::cout << "System time or timezone has changed!" << std::endl;
    // Notify Flutter of the time change
    g_autoptr(FlValue) result = fl_value_new_string("Time has changed");
    fl_method_channel_invoke_method(g_plugin_instance->channel, "timeChanged", result, nullptr, nullptr, nullptr);
}

static void* watch_time_changes(void* arg) {
    int inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        std::cerr << "Failed to initialize inotify!" << std::endl;
        return nullptr;
    }

    int watch_desc_localtime = inotify_add_watch(inotify_fd, "/etc/localtime", IN_MODIFY);
    int watch_desc_timezone = inotify_add_watch(inotify_fd, "/etc/timezone", IN_MODIFY);

    if (watch_desc_localtime < 0 || watch_desc_timezone < 0) {
        std::cerr << "Failed to add inotify watch!" << std::endl;
        close(inotify_fd);
        return nullptr;
    }

    const size_t event_size = sizeof(struct inotify_event);
    const size_t buf_len = 1024 * (event_size + 16);
    char buffer[buf_len];

    while (true) {
        int length = read(inotify_fd, buffer, buf_len);
        if (length < 0) {
            std::cerr << "Error reading from inotify file descriptor!" << std::endl;
            break;
        }

        int i = 0;
        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                if (event->mask & IN_MODIFY) {
                    time_changed();
                }
            }
            i += event_size + event->len;
        }
    }

    close(inotify_fd);
    return nullptr;
}

FlMethodResponse* get_platform_version() {
  struct utsname uname_data = {};
  uname(&uname_data);
  g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
  g_autoptr(FlValue) result = fl_value_new_string(version);
  return FL_METHOD_RESPONSE(fl_method_success_response_new(result));
}

static void handle_method_call(SystemTimeChangeDetectorPlugin* self,
                               FlMethodCall* method_call) {
    const gchar* method = fl_method_call_get_name(method_call);

    if (strcmp(method, "getPlatformVersion") == 0) {
        g_autofree gchar *version = g_strdup_printf("Linux %s", glib_check_version(2, 0, 0));
        g_autoptr(FlValue) result = fl_value_new_string(version);
        fl_method_call_respond_success(method_call, result, nullptr);
    } else if (strcmp(method, "startListening") == 0) {
        std::thread(watch_time_changes, nullptr).detach();
        g_autoptr(FlValue) result = fl_value_new_string("Listening for time changes");
        fl_method_call_respond_success(method_call, result, nullptr);
    } else {
        fl_method_call_respond_not_implemented(method_call, nullptr);
    }
}

static void timechangedetector_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(timechangedetector_plugin_parent_class)->dispose(object);
}

static void timechangedetector_plugin_class_init(SystemTimeChangeDetectorPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = timechangedetector_plugin_dispose;
}

static void timechangedetector_plugin_init(SystemTimeChangeDetectorPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  SystemTimeChangeDetectorPlugin* plugin = TIMECHANGEDETECTOR_PLUGIN(user_data);
  timechangedetector_plugin_handle_method_call(plugin, method_call);
}

// Plugin registration
static FlMethodChannel *channel = NULL;
void timechangedetector_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  SystemTimeChangeDetectorPlugin* plugin = TIMECHANGEDETECTOR_PLUGIN(
      g_object_new(timechangedetector_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel = fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "systemtimechangedetector",
                            FL_METHOD_CODEC(codec));

  plugin->channel = channel;

    fl_method_channel_set_method_call_handler(channel,
                                              [](FlMethodChannel* channel, FlMethodCall* method_call, gpointer user_data) {
                                                  SystemTimeChangeDetectorPlugin* plugin = TIMECHANGEDETECTOR_PLUGIN(user_data);
                                                  handle_method_call(plugin, method_call);
                                              },
                                              g_object_ref(plugin),
                                              g_object_unref);

    fl_plugin_registrar_add_plugin(registrar, FL_PLUGIN(plugin));

//  fl_method_channel_set_method_call_handler(channel, method_call_cb,
//                                            g_object_ref(plugin),
//                                            g_object_unref);
//
//  g_object_unref(plugin);
}
