#include "timechangedetector_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <iostream>
#include <thread>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

// Function to handle time change event
void timeChanged(flutter::PluginRegistrarWindows* registrar) {
    static auto channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "timechangedetector",
                    &flutter::StandardMethodCodec::GetInstance());

    // Notify Flutter of the time change
    channel->InvokeMethod("timeChanged", std::make_unique<flutter::EncodableValue>("Time has changed"));
}

// Function to simulate listening for time changes
void listenForTimeChanges(flutter::PluginRegistrarWindows* registrar) {
    // Simulate listening for time changes
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        timeChanged(registrar);
    }
}

// Windows procedure to detect time changes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
static flutter::PluginRegistrarWindows* registrar = nullptr;

if (uMsg == WM_TIMECHANGE) {
if (registrar != nullptr) {
timeChanged(registrar);
}
}

return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Function to set up a window for listening to system messages
void setupTimeChangeListener(flutter::PluginRegistrarWindows* registrar) {
    const wchar_t CLASS_NAME[] = L"TimeChangeListenerWindow";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            L"Time Change Listener",
            0,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL
    );

    if (hwnd != NULL) {
        // Store registrar in the window procedure
        registrar = registrar;
        ShowWindow(hwnd, SW_HIDE); // Hide the window as it is not needed to be shown

        // Main message loop
        MSG msg = { };
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

