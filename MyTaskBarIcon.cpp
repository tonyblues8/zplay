#include "MyTaskBarIcon.h"
#include "Command.h"
#include <wx/msgdlg.h>
#include <wx/utils.h>
#define MY_BASE_ID (wxID_HIGHEST + 1)
enum {
    ID_MY_CUSTOM_ACTION = MY_BASE_ID,
    ID_ANOTHER_ACTION,
    ID_HELP,
    ID_ZDELV,
    ID_ABOUT,
    ID_SHOW_HIDE,
    ID_EXIT
};

wxBEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(wxID_EXIT, MyTaskBarIcon::OnExit)
    EVT_MENU(wxID_HELP, MyTaskBarIcon::OnHelp)
    EVT_MENU(wxID_ABOUT, MyTaskBarIcon::OnUsef)
    EVT_MENU(ID_MY_CUSTOM_ACTION, MyTaskBarIcon::OnLeftClicka)
    EVT_TASKBAR_LEFT_DCLICK(MyTaskBarIcon::OnLeftDoubleClick)
wxEND_EVENT_TABLE()

MyTaskBarIcon::MyTaskBarIcon(wxFrame* frame, wxFrame* frame2)
    : m_frame(frame), m_frame2(frame2) {
    wxMemoryInputStream memStreamIcon(icon_png, icon_png_len);
    wxImage imageIcon(memStreamIcon, wxBITMAP_TYPE_PNG);
    if (imageIcon.IsOk()) {
        wxIcon icon;
        icon.CopyFromBitmap(wxBitmap(imageIcon));
        SetIcon(icon, wxString::FromUTF8("熊猫下载v1.0.0"));
    }
}

wxMenu* MyTaskBarIcon::CreatePopupMenu() {
    wxMenu* menu = new wxMenu();
    menu->Append(wxID_HELP, wxString::FromUTF8("关于"));
    menu->Append(wxID_ABOUT, wxString::FromUTF8("帮助"));
    menu->AppendSeparator();
    menu->Append(ID_MY_CUSTOM_ACTION, wxString::FromUTF8("片头片尾|添加字幕片头/剪切广告片段"));
    menu->AppendSeparator();
    menu->Append(wxID_EXIT, wxString::FromUTF8("退出"));
    return menu;
}

void MyTaskBarIcon::OnExit(wxCommandEvent& event) {
    wxMessageDialog dialog(m_frame->IsShown() ? m_frame : m_frame2,
                           "确认结束程序?", "确认信息", wxYES_NO | wxICON_QUESTION);
    dialog.SetWindowStyle(dialog.GetWindowStyle() | wxSTAY_ON_TOP);
    if (dialog.ShowModal() == wxID_YES) {
        wxApp::GetInstance()->ExitMainLoop();
    }
}

void MyTaskBarIcon::OnHelp(wxCommandEvent& event) {
    wxLaunchDefaultBrowser(wxString::FromUTF8("https://mv.6deng.cn:8443/mobile/app"));
}

void MyTaskBarIcon::OnUsef(wxCommandEvent& event) {
    //wxMessageBox("ok1", "提示", wxOK | wxICON_INFORMATION);
        wxString homeDir = wxGetHomeDir();
        wxString filePath = homeDir + "/zsprundir/source/b/help.mp4";
        filePath = Command::NormalizePath(filePath.ToStdString());
        m_frame->SetWindowStyle(m_frame->GetWindowStyle() & ~wxSTAY_ON_TOP);
        Command::PlayVideo(filePath);
}

void MyTaskBarIcon::OnLeftClicka(wxCommandEvent& event) {
    ToggleWindowVisibility();
}

void MyTaskBarIcon::OnLeftClick(wxTaskBarIconEvent&) {
    wxMessageBox("ok", "提示", wxOK | wxICON_INFORMATION);
    ToggleWindowVisibility();
}

void MyTaskBarIcon::OnLeftDoubleClick(wxTaskBarIconEvent&) {
    wxMessageBox("ok2", "提示", wxOK | wxICON_INFORMATION);
    ToggleWindowVisibility();
}

void MyTaskBarIcon::ToggleWindowVisibility() {
    if (m_frame && m_frame2) {
        if (!m_frame->IsShown() && !m_frame2->IsShown()) {
            m_frame->Show();
            m_frame->Raise();
            m_frame->SetWindowStyle(m_frame->GetWindowStyle() | wxSTAY_ON_TOP);
            Command::CenterWindow(m_frame);
        } else if (m_frame->IsShown() && !m_frame2->IsShown()) {
            m_frame->Hide();
            m_frame2->Show();
            m_frame2->Raise();
            m_frame2->SetWindowStyle(m_frame2->GetWindowStyle() | wxSTAY_ON_TOP);
            Command::CenterWindow(m_frame2);
        } else if (!m_frame->IsShown() && m_frame2->IsShown()) {
            m_frame2->Hide();
            m_frame->Show();
            m_frame->Raise();
            m_frame->SetWindowStyle(m_frame->GetWindowStyle() | wxSTAY_ON_TOP);
            Command::CenterWindow(m_frame);
        } else {
            m_frame->Hide();
            m_frame2->Hide();
        }
    }
}


