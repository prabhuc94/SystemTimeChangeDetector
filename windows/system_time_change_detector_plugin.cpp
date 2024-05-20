#include "system_time_change_detector_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

#include <Windows.h> // Include Windows.h for OutputDebugString

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>
#include <thread>
#include <iostream>

namespace system_time_change_detector {

    // Global pointer to store the registrar
    flutter::PluginRegistrarWindows *g_registrar = nullptr;

// Function to handle time change event
    void timeChanged() {
        if ((g_registrar != nullptr)) {
            OutputDebugString(L"EMITTING DATA CHANNEL CREATING\n"); // Print debug message
            auto channel = std::make_unique < flutter::MethodChannel < flutter::EncodableValue >> (
                    g_registrar->messenger(), "systemtimechangedetector",
                            &flutter::StandardMethodCodec::GetInstance());

            OutputDebugString(L"EMITTED DATA\n"); // Print debug message
            // Notify Flutter of the time change
            channel->InvokeMethod("timeChanged",
                                  std::make_unique<flutter::EncodableValue>("Time has changed"));
        } else {
            OutputDebugString(L"g_registrar is null\n"); // Print debug message
        }
    }

    // Window Procedure
    LRESULT CALLBACK
    WindowProc(HWND
    hwnd,
    UINT uMsg, WPARAM
    wParam,
    LPARAM lParam
    ) {
    OutputDebugString(L"Time CONDITION Checking\n"); // Print debug message
    if (uMsg == WM_TIMECHANGE) {
    timeChanged();
}
return
DefWindowProc(hwnd, uMsg, wParam, lParam
);
}

// Function to set up a window for listening to system messages
void setupTimeChangeListener(flutter::PluginRegistrarWindows *registrar) {
    g_registrar = registrar;
    const wchar_t CLASS_NAME[] = L"TimeChangeListenerWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    OutputDebugString(L"registration checking\n"); // Print debug message
    if (!RegisterClass(&wc)) {
        OutputDebugString(L"registration failure\n"); // Print debug message
        // Handle registration failure
        return;
    }
    HWND hwnd = CreateWindowEx(
            0, CLASS_NAME, L"Time Change Listener", 0,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, GetModuleHandle(NULL), NULL
    );

    OutputDebugString(L"window creation going on\n"); // Print debug message
    if (hwnd == NULL) {
        // Handle window creation failure
        OutputDebugString(L"window creation failure\n"); // Print debug message
        return;
    }

    ShowWindow(hwnd, SW_HIDE); // Hide the window as it is not needed to be shown

    // Main message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message != WM_QUIT) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

// static
void SystemTimeChangeDetectorPlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarWindows *registrar) {
    auto channel = std::make_unique < flutter::MethodChannel < flutter::EncodableValue >> (
            registrar->messenger(), "systemtimechangedetector",
                    &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<SystemTimeChangeDetectorPlugin>();

    channel->SetMethodCallHandler(
            [plugin_pointer = plugin.get(), registrar](const auto &call, auto result) {
                plugin_pointer->HandleMethodCall(call, std::move(result), registrar);
            });

    registrar->AddPlugin(std::move(plugin));

}

SystemTimeChangeDetectorPlugin::SystemTimeChangeDetectorPlugin() {}

SystemTimeChangeDetectorPlugin::~SystemTimeChangeDetectorPlugin() {}

void SystemTimeChangeDetectorPlugin::HandleMethodCall(
        const flutter::MethodCall <flutter::EncodableValue> &method_call,
        std::unique_ptr <flutter::MethodResult<flutter::EncodableValue>> result,
        flutter::PluginRegistrarWindows *registrar) {
    if (method_call.method_name().compare("getPlatformVersion") == 0) {
        std::ostringstream version_stream;
        version_stream << "Windows ";
        if (IsWindows10OrGreater()) {
            version_stream << "10+";
        } else if (IsWindows8OrGreater()) {
            version_stream << "8";
        } else if (IsWindows7OrGreater()) {
            version_stream << "7";
        }
        result->Success(flutter::EncodableValue(version_stream.str()));
    } else if (method_call.method_name().compare("startListening") == 0) {
        std::thread([result = std::move(result), registrar]() {
            OutputDebugString(L"LISTEN\n"); // Print debug message
            setupTimeChangeListener(registrar); // Start listening for time changes
            result->Success(flutter::EncodableValue(
                    "startListening invoked")); // Notify Flutter that the method call was successful
        }).detach();
    } else {
        result->NotImplemented();
    }
}

}  // namespace systemtimechangedetector
