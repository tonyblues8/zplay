#ifndef THEMELISTENER_H
#define THEMELISTENER_H

#include <wx/wx.h>

#include "zdelv.h"

class ThemeListener {
public:
    static void Register();   // 注册监听
    static void UpdateTheme(); // 主题变化时更新界面

};

#endif // THEMELISTENER_H
