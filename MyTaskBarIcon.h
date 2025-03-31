#ifndef MYTASKBARICON_H
#define MYTASKBARICON_H

#include <wx/wx.h>
#include <wx/taskbar.h>
#include <wx/mstream.h> // 处理 wxMemoryInputStream
#include "pic/icon.h" // 你用于加载 `icon_png` 资源的头文件
//void CenterWindow(wxWindow* window);


class MyTaskBarIcon : public wxTaskBarIcon {
public:
    MyTaskBarIcon(wxFrame* frame, wxFrame* frame2);
    virtual wxMenu* CreatePopupMenu() override;

    void OnExit(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnUsef(wxCommandEvent& event);
    void OnLeftClicka(wxCommandEvent& event);
    void OnLeftClick(wxTaskBarIconEvent&);
    void OnLeftDoubleClick(wxTaskBarIconEvent&);
    void ToggleWindowVisibility();

    wxDECLARE_EVENT_TABLE();

private:
    wxFrame* m_frame;
    wxFrame* m_frame2;
};

#endif // MYTASKBARICON_H
