#ifndef MY_PROCESS_H
#define MY_PROCESS_H
#include <wx/wx.h>
#include <wx/process.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/msgdlg.h>

//void CenterWindow(wxWindow* window);
//void CenterWindow(wxWindow* window);

class MyProcess : public wxProcess {
public:
    MyProcess(wxEvtHandler* parent, wxTimer* timer1, wxFrame* frame, const wxString& filePath);

    virtual void OnTerminate(int pid, int status) override;
    bool IsAlive() const;
    wxString m_filePath;

private:
    void HandleSuccess(wxFrame* frame);  // 处理成功终止
    void HandleSuccess(wxFrame* m_frame, wxString filePath);
    void HandleFailure(int pid, int status);  // 处理失败终止
    void RestoreFrame();   // 还原窗口
    //void CenterWindow(wxFrame* frame);  // 窗口居中
    //wxString m_filePath;

    wxTimer* m_timer1;
    wxFrame* m_frame;
};

#endif // MY_PROCESS_H
