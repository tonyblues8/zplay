#include "ThemeListener.h"
#include "Command.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#elif _WIN32
#include <windows.h>
#elif __linux__
#include <gio/gio.h>
#endif


void ThemeListener::UpdateTheme() {
    wxGetApp().CallAfter([]() {
        wxFrame* mainFrame = wxGetApp().GetMainFrame();
        wxFrame* mainFrame2 = wxGetApp().GetMainFrame2();
        wxPanel* Getpanel = wxGetApp().Getpanel();
        wxPanel* Getpanel2 = wxGetApp().Getpanel2();
        if (mainFrame) {
            Command::SetColorsAndStylesRecursiveN(mainFrame, Getpanel);
        }
        if (mainFrame2) {
            Command::SetColorsAndStylesRecursiveN(mainFrame2, Getpanel2);
        }
    });
}

void ThemeListener::Register() {
#ifdef __APPLE__
    // macOS: 监听 AppleInterfaceThemeChangedNotification
    CFNotificationCenterRef center = CFNotificationCenterGetDistributedCenter();
    CFNotificationCenterAddObserver(
        center, nullptr,
        [](CFNotificationCenterRef, void*, CFStringRef, const void*, CFDictionaryRef) {
            UpdateTheme();
        },
        CFSTR("AppleInterfaceThemeChangedNotification"),
        nullptr,
        CFNotificationSuspensionBehaviorDeliverImmediately // 立即传递通知
    );

#elif _WIN32
    // Windows: 监听 WM_SETTINGCHANGE
    static WNDPROC oldWndProc;
    HWND hwnd = GetActiveWindow();
    oldWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)+[](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (msg == WM_SETTINGCHANGE && lParam && wcscmp((LPCWSTR)lParam, L"ImmersiveColorSet") == 0) {
            UpdateTheme();
        }
        return CallWindowProc(oldWndProc, hWnd, msg, wParam, lParam);
    });

#elif __linux__
    // Linux (GTK): 监听 gsettings 主题变化
    static GSettings* settings = g_settings_new("org.gnome.desktop.interface");
    g_signal_connect(settings, "changed::gtk-theme", G_CALLBACK([](GSettings*, gchar*, gpointer) {
        UpdateTheme();
    }), nullptr);
#endif
}
