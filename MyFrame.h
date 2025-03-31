#pragma once
#include <wx/tglbtn.h>
#include <wx/menu.h>
#include <wx/timer.h>
#include <wx/mstream.h>
#include <filesystem>
#include <wx/wx.h>
#include <wx/timer.h>
#include "MyProcess.h"  // 假设自定义进程类的头文件
#include "CustomButton.h" // 假设自定义按钮类的头文件

#ifdef __cplusplus
extern "C" {
#endif

// 只声明需要 C 语言链接的函数
bool IsDarkModeEnabled();

#ifdef __cplusplus
}
#endif

extern bool isDarkMode;
//extern wxString filePath;
extern wxPanel* panel;
extern wxPanel* m_panelSelectSizer2;

//extern wxBitmap bitmapbubj, bitmapbubj2, bitmapOn, bitmapOff;
//extern wxColour backgroundColor, textColor, customColor, customColor2;



namespace fs = std::filesystem;
//void CenterWindow(wxWindow* window);
//std::string normalizePath(const std::string& path);
//void commPlayVideo(const wxString& filePath);
//wxString ClearAllSpace(const wxString &str);
//void SetColorsAndStylesRecursive(wxWindow* window,wxPanel* Getpanel);

// 事件ID枚举

class MyFrame : public wxFrame {
public:
    MyFrame();

private:
    // 事件处理函数
    void OnToggleButtonClick3(wxCommandEvent& event);
    void OnSelectPtButtonClick(wxCommandEvent& event);
    void OnSelectZmButtonClick(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnShowHide(wxCommandEvent& event);
    void OnButtonClick(wxCommandEvent& event);
    void KillProcess(wxCommandEvent& event);
    void OnTimer5(wxTimerEvent& event);
    void OnSelectPathButtonClick(wxCommandEvent& event);
    void OnToggleButtonClick(wxCommandEvent& event);

    // 辅助函数
    void RunCommandWithOutput(const wxString& command, int refreshTime, const wxString& filepath);
    void ReadAndDisplayStream(wxInputStream* stream, const wxString& prefix);
    wxString RemoveAnsiEscapeCodes(const wxString& input);
    double ExtractProgressPercentage(const wxString& output);

    // UI控件成员变量
    wxBoxSizer* mainSizer;
    wxTextCtrl* m_outputTextCtrl;
    wxTextCtrl* m_dirPathTextCtrl3wx;
    wxTextCtrl* m_dirPathTextCtrl3;
    wxTextCtrl* m_dirPathTextCtrl4;

    wxBoxSizer* selectSizer;
    wxBoxSizer* selectSizer2;
    wxBoxSizer* inputSizer2;

    CustomButton* selectDirButton3wx;
    CustomButton* selectDirButton3;
    CustomButton* selectDirButton4;
    wxBitmapToggleButton* m_toggleButton8;
    wxBitmapToggleButton* m_toggleButton2;

    wxTextCtrl* m_textCtrl;
    wxTextCtrl* m_textCtrl2;

    MyProcess* process;
    wxTimer m_timer5;
    CustomButton* startButton;
    CustomButton* stopButton;

    wxBoxSizer* controlSizer;
    //wxPanel* m_panelSelectSizer2;
    //wxPanel* panel;

    // 样式相关成员
wxBitmap bitmapbubj;
wxBitmap bitmapbubj2;

//wxString filePath;
wxColour backgroundColor;
wxColour textColor;
wxColour customColor;
wxColour customColor2;

wxBitmap bitmapOff;
wxBitmap bitmapOn;
wxString filePath;
bool isDarkMode;
    wxDECLARE_EVENT_TABLE();
};