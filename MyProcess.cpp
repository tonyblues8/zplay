#include "MyProcess.h"
#include "Command.h"
#include <wx/msgdlg.h>
#include <wx/utils.h>

MyProcess::MyProcess(wxEvtHandler* parent, wxTimer* timer1, wxFrame* frame, const wxString& filePath)
    : wxProcess(parent), m_timer1(timer1), m_frame(frame), m_filePath(filePath) {
    Redirect(); // 启用重定向
}

void MyProcess::OnTerminate(int pid, int status) {
    if (status == 0) {
        HandleSuccess(m_frame, m_filePath);
    } else {
        HandleFailure(pid, status);
    }

    // 停止定时器
    //if (m_timer1) {
    //    m_timer1->Stop();
    //}
}

bool MyProcess::IsAlive() const {
    return wxProcess::Exists(GetPid());
}

void MyProcess::HandleSuccess(wxFrame* m_frame,wxString m_filePath) {
    RestoreFrame();
    Command zycmd;
    std::string filepath = m_filePath.ToStdString();
    zycmd.ReplaceAll(filepath, " ", "");
    zycmd.ReplaceAll(filepath, "-P", "");
    //wxMessageBox(filepath, "提示", wxOK | wxICON_INFORMATION);
    m_frame->SetWindowStyle(m_frame->GetWindowStyle() & ~wxSTAY_ON_TOP);
    wxMessageDialog dialog(m_frame,"任务成功结束,是否打开处理后的文件?", "确认信息", wxYES_NO | wxICON_QUESTION);
    dialog.SetWindowStyle(dialog.GetWindowStyle() | wxSTAY_ON_TOP);
    if (dialog.ShowModal() == wxID_YES) {
        m_frame->SetWindowStyle(m_frame->GetWindowStyle() & ~wxSTAY_ON_TOP);
        wxString command = wxString::Format("open %s", filepath);
        wxExecute(command, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
    }
}

void MyProcess::HandleFailure(int pid, int status) {
    RestoreFrame();
    wxMessageBox(wxString::Format("任务结束，status: %d, pid: %d", status,pid), "提示", wxOK | wxICON_ERROR);
}

void MyProcess::RestoreFrame() {
    if (!m_frame) return;

    bool isShown = m_frame->IsShown();
    bool isIconized = m_frame->IsIconized();

    if (!isShown) {
        m_frame->Show();
        m_frame->Raise();
    } else if (isIconized) {
        m_frame->Iconize(false);
        m_frame->Raise();
    }

    Command::CenterWindow(m_frame);
    m_frame->SetWindowStyle(m_frame->GetWindowStyle() | wxSTAY_ON_TOP);
}
/*
void MyProcess::CenterWindow(wxFrame* frame) {
    // 获取屏幕的大小
  if(frame){
    wxScreenDC screenDC;
    int screenWidth = screenDC.GetSize().x;
    int screenHeight = screenDC.GetSize().y;

    // 获取窗口的大小
    wxSize windowSize = frame->GetSize();
    int windowWidth = windowSize.x;
    int windowHeight = windowSize.y;

    // 计算窗口的位置
    int xPos = (screenWidth - windowWidth) / 2;
    int yPos = (screenHeight - windowHeight) / 2;

    // 设置窗口位置
    frame->Move(xPos, yPos);
  }
}
*/