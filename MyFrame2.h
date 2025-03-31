#ifndef MYFRAME2_H
#define MYFRAME2_H

#include <wx/wx.h>
#include <wx/tglbtn.h>
#include <wx/menu.h>
#include <wx/timer.h>
#include <wx/mstream.h>
#include <filesystem>
#include "CustomButton.h"
#include "MyProcess.h"

#ifdef __cplusplus
extern "C" {
#endif

// 只声明需要 C 语言链接的函数
//bool IsDarkModeEnabled();

#ifdef __cplusplus
}
#endif

extern bool isDarkMode;
//extern wxString filePath;
extern wxPanel* panel2;
//extern wxBitmap bitmapbubj, bitmapbubj2, bitmapOn, bitmapOff;
//extern wxColour backgroundColor, textColor, customColor, customColor2;

namespace fs = std::filesystem;
//void CenterWindow(wxWindow* window);
//std::string normalizePath(const std::string& path);
//void commPlayVideo(const wxString& filePath);
//wxString ClearAllSpace(const wxString &str);
//void SetColorsAndStylesRecursive(wxWindow* window,wxPanel* Getpanel);

class MyFrame2 : public wxFrame {
public:
    MyFrame2();
    //~MyFrame2();

private:
    void OnHelp(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnShowHide(wxCommandEvent& event);
    void OnToggleButtonClick(wxCommandEvent& event);
    void OnButtonClick(wxCommandEvent& event);
    void OnSelectDirButtonClick(wxCommandEvent& event);
    void OnSelectPathButtonClick(wxCommandEvent& event);
    void OnTimer5(wxTimerEvent& event);
    void KillProcess(wxCommandEvent& event);

    void RunCommandWithOutput(const wxString& command, int refreshTime, const wxString& filepath);
    void ReadAndDisplayStream(wxInputStream* stream, const wxString& prefix);
    wxString RemoveAnsiEscapeCodes(const wxString& input);

    // 成员变量
    //wxPanel* panel2;
    wxBoxSizer *mainSizer, *controlSizer, *selectSizer, *inputSizer2;
    wxTextCtrl *m_dirPathTextCtrl3wx, *m_dirPathTextCtrl3wx2, *m_outputTextCtrl;
    CustomButton *selectDirButton3wx, *selectDirButton3wx2, *startButton, *stopButton;
    wxBitmapToggleButton *m_toggleButton8;
    //wxBitmap bitmapbubj, bitmapbubj2, bitmapOn, bitmapOff;
    //wxColour backgroundColor, textColor, customColor, customColor2;
    wxTimer m_timer5;
    MyProcess* process;
    //wxString filePath;


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

#endif // MYFRAME2_H
