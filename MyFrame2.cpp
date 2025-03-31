#include "MyFrame2.h"
#include "AutoCloseDialog.h"
#include "Command.h"
#include <filesystem>
#include <wx/stdpaths.h>
#include <wx/regex.h>
#include "pic/yybj.h"
#include "pic/yybj2.h"
#include "pic/bubj.h"
#include "pic/bubj2.h"
#include "pic/imageon.h"
#include "pic/imageoff.h"
#include "pic/imageon1.h"
#include "pic/imageoff1.h"
//extern const unsigned char bubj_png[];
//extern const unsigned int bubj_png_len;
//extern const unsigned char bubj2_png[];
//extern const unsigned int bubj2_png_len;
//extern const unsigned char imageon_png[];
//extern const unsigned int imageon_png_len;
//extern const unsigned char imageoff_png[];
//extern const unsigned int imageoff_png_len;
//extern const unsigned char imageon1_png[];
//extern const unsigned int imageon1_png_len;
//extern const unsigned char imageoff1_png[];
//extern const unsigned int imageoff1_png_len;
#define MY_BASE_ID (wxID_HIGHEST + 1)
enum {
    ID_MY_CUSTOM_ACTION = MY_BASE_ID,
    ID_ANOTHER_ACTION,
    ID_HELP,
    ID_ABOUT,
    ID_SHOW_HIDE,
    ID_EXIT
};
wxBEGIN_EVENT_TABLE(MyFrame2, wxFrame)
    EVT_MENU(ID_EXIT, MyFrame2::OnExit)
    EVT_MENU(ID_HELP, MyFrame2::OnHelp)
    EVT_MENU(ID_ABOUT, MyFrame2::OnAbout)
    EVT_MENU(ID_SHOW_HIDE, MyFrame2::OnShowHide)
    EVT_BUTTON(wxID_ANY, MyFrame2::OnButtonClick)
    EVT_BUTTON(wxID_ANY, MyFrame2::KillProcess)
    EVT_BUTTON(wxID_ANY, MyFrame2::OnSelectPathButtonClick)
    EVT_BUTTON(wxID_ANY, MyFrame2::OnSelectDirButtonClick)
    EVT_TIMER(wxID_ANY, MyFrame2::OnTimer5)
    EVT_CLOSE(MyFrame2::OnClose)
wxEND_EVENT_TABLE()

MyFrame2::MyFrame2() : wxFrame(nullptr, wxID_ANY, wxString::FromUTF8("熊猫剪切 v1.0.0[剪切广告片段]"),
                                wxDefaultPosition, wxSize(800, 360),
                                wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
                                process(nullptr), m_timer5(this) {
    isDarkMode = Command::IsDarkMode();

    Command::CenterWindow(this);
    //this->Centre();

    wxMenu* menu = new wxMenu();
    menu->Append(ID_HELP, wxString::FromUTF8("帮助"));
    menu->Append(ID_ABOUT, wxString::FromUTF8("关于"));
    menu->AppendSeparator(); // 添加分割线
    menu->Append(ID_SHOW_HIDE, wxString::FromUTF8("显示/隐藏"));
    menu->AppendSeparator(); // 添加分割线
    menu->Append(ID_EXIT, wxString::FromUTF8("退出"));

    // 创建菜单栏
    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menu, wxString::FromUTF8("菜单"));

    SetMenuBar(menuBar);

    // 绑定菜单事件
    Bind(wxEVT_MENU, &MyFrame2::OnExit, this, ID_EXIT);
    Bind(wxEVT_MENU, &MyFrame2::OnHelp, this, ID_HELP);
    Bind(wxEVT_MENU, &MyFrame2::OnAbout, this, ID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame2::OnShowHide, this, ID_SHOW_HIDE);

    panel2 = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* mainSizerall = new wxBoxSizer(wxVERTICAL);
    //inputSizer = new wxBoxSizer(wxHORIZONTAL);
    //buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    controlSizer = new wxBoxSizer(wxHORIZONTAL);
    selectSizer = new wxBoxSizer(wxHORIZONTAL);
    inputSizer2 = new wxBoxSizer(wxHORIZONTAL);

    //backgroundColor = isDarkMode ? wxColour(36, 36, 36) : wxColour(242, 242, 242);
    backgroundColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    //textColor = isDarkMode ? wxColour(242, 242, 242) : wxColour(36, 36, 36);
    textColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    //customColor = isDarkMode ? wxColour(128, 128, 128) : wxColour(230, 230, 230);
    customColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    customColor2 = isDarkMode ? wxColour(168, 168, 168) : wxColour(242, 242, 242);

    this->SetBackgroundColour(backgroundColor);
    panel2->SetBackgroundColour(backgroundColor); // 浅蓝色

    wxMemoryInputStream memStreambubj(bubj_png, bubj_png_len);
    wxImage imagebubj(memStreambubj, wxBITMAP_TYPE_PNG);
    if (imagebubj.IsOk()) {
        wxImage scaledImagebubj = imagebubj.Rescale(120, 30, wxIMAGE_QUALITY_HIGH);
        bitmapbubj = wxBitmap(scaledImagebubj);
    }
    wxMemoryInputStream memStreambubj2(bubj2_png, bubj2_png_len);
    wxImage imagebubj2(memStreambubj2, wxBITMAP_TYPE_PNG);
    if (imagebubj2.IsOk()) {
        wxImage scaledImagebubj2 = imagebubj2.Rescale(120, 30, wxIMAGE_QUALITY_HIGH);
        bitmapbubj2 = wxBitmap(scaledImagebubj2);
    }

    if (isDarkMode){
        wxMemoryInputStream memStreamOn(imageon1_png, imageon1_png_len);
        wxMemoryInputStream memStreamOff(imageoff1_png, imageoff1_png_len);
        wxImage imageOn(memStreamOn, wxBITMAP_TYPE_PNG);
        wxImage imageOff(memStreamOff, wxBITMAP_TYPE_PNG);
        if (imageOn.IsOk() && imageOff.IsOk()) {
            wxImage scaledImageOn = imageOn.Rescale(30, 30, wxIMAGE_QUALITY_HIGH);
            bitmapOn = wxBitmap(scaledImageOn);
            wxImage scaledImageOff = imageOff.Rescale(30, 30, wxIMAGE_QUALITY_HIGH);
            bitmapOff = wxBitmap(scaledImageOff);
        }
    }else{
        wxMemoryInputStream memStreamOn(imageon_png, imageon_png_len);
        wxMemoryInputStream memStreamOff(imageoff_png, imageoff_png_len);
        wxImage imageOn(memStreamOn, wxBITMAP_TYPE_PNG);
        wxImage imageOff(memStreamOff, wxBITMAP_TYPE_PNG);
        if (imageOn.IsOk() && imageOff.IsOk()) {
            wxImage scaledImageOn = imageOn.Rescale(30, 30, wxIMAGE_QUALITY_HIGH);
            bitmapOn = wxBitmap(scaledImageOn);
            wxImage scaledImageOff = imageOff.Rescale(30, 30, wxIMAGE_QUALITY_HIGH);
            bitmapOff = wxBitmap(scaledImageOff);
        }
    }

    m_dirPathTextCtrl3wx = new wxTextCtrl(panel2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_dirPathTextCtrl3wx->SetMinSize(wxSize(260, 30));
    m_dirPathTextCtrl3wx->SetForegroundColour(textColor);
    m_dirPathTextCtrl3wx->SetOwnBackgroundColour(backgroundColor);
    selectDirButton3wx = new CustomButton(panel2, wxID_ANY, wxString::FromUTF8("文件路径"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    selectDirButton3wx->SetForegroundColour(textColor);
    selectDirButton3wx->SetOwnBackgroundColour(backgroundColor);

    m_dirPathTextCtrl3wx2 = new wxTextCtrl(panel2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_dirPathTextCtrl3wx2->SetMinSize(wxSize(260, 30));
    m_dirPathTextCtrl3wx2->SetForegroundColour(textColor);
    m_dirPathTextCtrl3wx2->SetOwnBackgroundColour(backgroundColor);
    selectDirButton3wx2 = new CustomButton(panel2, wxID_ANY, wxString::FromUTF8("样本音频路径"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    selectDirButton3wx2->SetForegroundColour(textColor);
    selectDirButton3wx2->SetOwnBackgroundColour(backgroundColor);

    //m_outputTextCtrl = new wxTextCtrl(panel2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY);
    m_outputTextCtrl = new wxTextCtrl(panel2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY | wxTE_NO_VSCROLL | wxTE_NOHIDESEL);
    m_outputTextCtrl->SetMinSize(wxSize(300, 120));
    m_outputTextCtrl->SetForegroundColour(textColor);
    m_outputTextCtrl->SetOwnBackgroundColour(backgroundColor);

    //startButton = new wxButton(panel2, wxID_ANY, wxString::FromUTF8("开始处理"));
    startButton = new CustomButton(panel2, wxID_ANY, wxString::FromUTF8("开始处理"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    startButton->SetMinSize(wxSize(120, 30));
    startButton->SetForegroundColour(textColor);
    startButton->SetOwnBackgroundColour(backgroundColor);

    stopButton = new CustomButton(panel2, wxID_ANY, wxString::FromUTF8("终止处理"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    stopButton->SetMinSize(wxSize(120, 30));
    stopButton->SetForegroundColour(textColor);
    stopButton->SetOwnBackgroundColour(backgroundColor);

    wxStaticText* staticTexth = new wxStaticText(panel2, wxID_ANY, wxString::FromUTF8("H265编码"));
    staticTexth->SetForegroundColour(textColor);
    //if (!isDarkMode){
    //    m_toggleButton8 = new wxBitmapToggleButton(panel2, wxID_ANY, bitmapOn, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    //    m_toggleButton8->SetValue(true);
    //}else{
    m_toggleButton8 = new wxBitmapToggleButton(panel2, wxID_ANY, bitmapOff, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    m_toggleButton8->SetValue(false);
    //}
    m_toggleButton8->SetMinSize(wxSize(30, 30));

    selectSizer->Add(selectDirButton3wx, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer->Add(m_dirPathTextCtrl3wx, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer->Add(selectDirButton3wx2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer->Add(m_dirPathTextCtrl3wx2, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    controlSizer->Add(startButton, 0, wxALL, 5);
    controlSizer->Add(stopButton, 0, wxALL, 5);
    controlSizer->Add(staticTexth, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlSizer->Add(m_toggleButton8, 0, wxALL, 5);

    mainSizer->Add(inputSizer2, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(selectSizer, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(m_outputTextCtrl, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(controlSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

    m_toggleButton8->Bind(wxEVT_TOGGLEBUTTON, &MyFrame2::OnToggleButtonClick, this);
    Bind(wxEVT_BUTTON, &MyFrame2::OnButtonClick, this, startButton->GetId());
    Bind(wxEVT_BUTTON, &MyFrame2::KillProcess, this, stopButton->GetId());
    Bind(wxEVT_BUTTON, &MyFrame2::OnSelectPathButtonClick, this, selectDirButton3wx->GetId());
    Bind(wxEVT_BUTTON, &MyFrame2::OnSelectDirButtonClick, this, selectDirButton3wx2->GetId());
    Bind(wxEVT_TIMER, &MyFrame2::OnTimer5, this, m_timer5.GetId());
    Bind(wxEVT_CLOSE_WINDOW, &MyFrame2::OnClose, this);


    //mainSizerall->Add(panel2, 1, wxEXPAND);
    mainSizerall->Add(panel2, 0, wxALIGN_CENTER | wxFIXED_MINSIZE, 0);
    panel2->SetMinSize(wxSize(800, 360));
    Command::SetColorsAndStylesRecursiveN(this,panel2);
    SetSizer(mainSizerall);
    //m_panelSelectSizer2->Layout();
    //panel2->SetSizer(mainSizer);
    panel2->SetSizerAndFit(mainSizer);
    panel2->Layout();
    panel2->Refresh();
}

// 其他事件处理函数...


    // 菜单事件处理
    void MyFrame2::OnHelp(wxCommandEvent& event) {
        wxString homeDir = wxGetHomeDir();
        wxString filePath = homeDir + "/zsprundir/source/b/help.mp4";
        filePath = Command::NormalizePath(filePath.ToStdString());
        Command::PlayVideo(filePath);
    }

    void MyFrame2::OnAbout(wxCommandEvent& event) {
        wxLaunchDefaultBrowser(wxString::FromUTF8("https://mv.6deng.cn:8443/mobile/app"));
    }

    void MyFrame2::OnExit(wxCommandEvent& event) {
        wxApp::GetInstance()->ExitMainLoop();
    }

    void MyFrame2::OnClose(wxCloseEvent& event) {
        //this->Hide();
        this->Iconize(true);
        this->SetWindowStyle(this->GetWindowStyle() & ~wxSTAY_ON_TOP);
        event.Veto();
    }

    void MyFrame2::OnShowHide(wxCommandEvent& event) {
            if (this->IsIconized()) {
                this->Iconize(false);
            } else {
                this->Iconize(true);
            }
            //if (this->IsShown()) {
            //    this->Hide();
            //} else {
            //    this->Show();
            //    this->Raise();
            //}
    }
    // 封装后的通用方法
void MyFrame2::RunCommandWithOutput(const wxString& command, int refreshTime, const wxString& filepath) {
    // 创建自定义进程
    process = new MyProcess(this,&m_timer5,this,filepath);
    long pid = wxExecute(command, wxEXEC_ASYNC, process);
    //std::unique_ptr<MyProcess> process = std::make_unique<MyProcess>(this, &m_timer5, this);
    //long pid = wxExecute(command, wxEXEC_ASYNC, process.get());

    if (pid == 0) {
        m_outputTextCtrl->Clear();
        m_outputTextCtrl->AppendText("执行错误。\n");
        //delete process;
        //process = nullptr;
    } else {
        m_outputTextCtrl->Clear();
        m_outputTextCtrl->AppendText("开始处理...\n");
        m_timer5.Start(refreshTime); // 每 100ms 检查一次输出
    }
}



void MyFrame2::KillProcess(wxCommandEvent&) {
    if (process) {
        long pid = process->GetPid(); // 获取进程 ID
        if (pid > 0) {
            wxKillError killError;  // 创建 wxKillError 对象

            // 先查找子进程
            wxArrayString output;
            if (wxExecute(wxString::Format("pgrep -P %ld", pid), output) == 0) {
                for (const auto& childPidStr : output) {
                    long childPid;
                    if (childPidStr.ToLong(&childPid)) {
                        if (wxKill(childPid, wxSIGTERM, &killError) == 0) {
                            m_outputTextCtrl->AppendText(wxString::Format("子进程 %ld 已终止。\n", childPid));
                        } else {
                            m_outputTextCtrl->AppendText(wxString::Format("终止子进程 %ld 失败。\n", childPid));
                        }
                    }
                }
            } else {
                m_outputTextCtrl->AppendText("没有找到子进程。\n");
            }

            // 最后终止主进程
            if (wxKill(pid, wxSIGTERM, &killError) == 0) {
                m_outputTextCtrl->AppendText(wxString::Format("主进程 %ld 已终止。\n",pid));
            } else {
                m_outputTextCtrl->AppendText(wxString::Format("终止主进程 %ld 失败。\n",pid));
            }
            wxString cmd = wxString::Format("sh -c \"ps -ef | grep -E 'ffmpeg|zdvq'| grep -v grep | awk '{print \\\"kill -15 \\\" $2 %ld}' | sh\"",pid);
            wxExecute(cmd, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE);



            //wxString cmd;
            //cmd = wxString::Format("kill -15 %d", pid); // 先尝试 SIGTERM
            //wxExecute(cmd, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE);
            //wxMilliSleep(500); // 等待 500ms
            //
            //// 检查进程是否仍在运行，如果是，则强制终止
            //cmd = wxString::Format("ps -p %d > /dev/null || kill -9 %d", pid, pid);
            //wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
            //
            //// 终止所有 ffmpeg 进程
            //wxString cmd2 = "sh -c \"ps -ef | grep ffmpeg | grep -v grep | awk '{print \\\"kill -15 \\\" $2}'| sh\"";
            //wxExecute(cmd2, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE);
            //wxMilliSleep(500); // 等待 500ms
            //
            ////m_outputTextCtrl->AppendText("进程已终止。\n");
        }
        //delete process;
        //process = nullptr;
    } else {
        //wxMessageBox("程序未运行", "提示", wxOK | wxICON_INFORMATION);
        AutoCloseDialog* dlg = new AutoCloseDialog(this, wxString::FromUTF8("程序未运行"), 3000);
        dlg->Show();
    }
}
void MyFrame2::OnTimer5(wxTimerEvent&) {
    if (process) {
        wxInputStream* stdoutStream = process->GetInputStream();
        wxInputStream* stderrStream = process->GetErrorStream();
        //m_outputTextCtrl->Clear();

        // 读取标准输出
        ReadAndDisplayStream(stdoutStream, "info: ");

        // 读取标准错误
        ReadAndDisplayStream(stderrStream, "info: ");

        // 检查进程是否已退出
        //if (process->IsInputOpened() && !process->IsAlive()) {
        if (!process->IsAlive()) {
            m_timer5.Stop();
            //delete process;
            process = nullptr;
            m_outputTextCtrl->AppendText("\n处理完成。\n");
        }
    }
}

void MyFrame2::ReadAndDisplayStream(wxInputStream* stream, const wxString& prefix) {
    if (stream && stream->CanRead()) {
        // 读取所有可用数据（非阻塞）
        char buffer[4096];
        while (stream->CanRead()) {
            stream->Read(buffer, sizeof(buffer));
            size_t bytesRead = stream->LastRead();
            if (bytesRead == 0) break;
            wxString output(buffer, bytesRead);
            output = RemoveAnsiEscapeCodes(output);
            if (output.Contains(wxT("█")) || output.Contains(wxT("|"))) {
                //m_outputTextCtrl->Clear();
                m_outputTextCtrl->AppendText(output);
            }else{
                m_outputTextCtrl->AppendText(prefix + output);
            }
        }
    }
}

wxString MyFrame2::RemoveAnsiEscapeCodes(const wxString& input) {
    static wxRegEx ansiEscapeRegex("\\x1B\\[[0-9;]*[mK]");
    wxString cleaned = input;
    ansiEscapeRegex.ReplaceAll(&cleaned, "");
    return cleaned;
}


    void MyFrame2::OnButtonClick(wxCommandEvent&) {
        if (process) {
            //wxMessageBox("程序已经运行", "提示", wxOK | wxICON_INFORMATION);
            AutoCloseDialog* dlg = new AutoCloseDialog(this, wxString::FromUTF8("程序已经运行"), 3000);
            dlg->Show();
            return;
        }
        wxString filePath0 = m_dirPathTextCtrl3wx->GetValue();
        filePath0 = Command::ClearAllSpace(Command::NormalizePath(filePath0.ToStdString()));
        //wxString filePath;
        if (!fs::exists(filePath0.ToStdString())) {
            wxString message = wxString::Format("文件路径不存在: %s", filePath0);
            wxMessageBox(message, "提示", wxOK | wxICON_INFORMATION);
            return;
        }
        if (filePath0.IsEmpty()) {
            wxMessageBox("文件路径是必须的", "提示", wxOK | wxICON_INFORMATION);
            return;
        }else{
            filePath = filePath0;
        }

        int h265tag = 4;
        if (m_toggleButton8->GetValue()) {
        	h265tag = 5;
        }

        wxString afilePath0 = m_dirPathTextCtrl3wx2->GetValue();
        afilePath0 = Command::ClearAllSpace(Command::NormalizePath(afilePath0.ToStdString()));
        if (!fs::exists(afilePath0.ToStdString())) {
            wxString message = wxString::Format("样本音频路径不存在: %s", afilePath0);
            wxMessageBox(message, "提示", wxOK | wxICON_INFORMATION);
            return;
        }


        //wxMessageBox(wxString::Format("%d", h265tag), "提示", wxOK | wxICON_INFORMATION);
        wxStandardPaths& stdPaths = wxStandardPaths::Get();
        wxString resourceexecutableFilePath = stdPaths.GetResourcesDir() + "/zdvq";
        const std::string pythonPath = "/usr/local/bin/python3";
        wxString command;
        command = wxString::Format("%s %s %s %d %s", pythonPath, resourceexecutableFilePath, filePath, h265tag, afilePath0);
        int refreshTime = 50;
        RunCommandWithOutput(command, refreshTime, filePath);
    }

    void MyFrame2::OnSelectDirButtonClick(wxCommandEvent& event) {
        wxDirDialog dirDlg(this, wxString::FromUTF8("选择保存目录"), "", wxDD_DIR_MUST_EXIST);
        if (dirDlg.ShowModal() == wxID_OK) {
            m_dirPathTextCtrl3wx2->SetValue(dirDlg.GetPath());
        }
    }

    void MyFrame2::OnSelectPathButtonClick(wxCommandEvent& event) {
        // 创建一个选择类型的对话框
        wxArrayString choices;
        choices.Add("选择文件");
        choices.Add("选择目录");

        wxSingleChoiceDialog typeDlg(this,
                                    "请选择您要选取的类型：",
                                    "选择文件或目录",
                                    choices);

        if (typeDlg.ShowModal() == wxID_OK)
        {
            wxString path;
            int selection = typeDlg.GetSelection();

            if (selection == 0) // 文件选择
            {
                wxFileDialog fileDlg(this,
                                    "选择文件",
                                    "",
                                    "",
                                    wxString::FromUTF8("媒体文件 (*.mp4;*.mov;*.wav;*.mp3;*.aac;*.m4a)|*.mp4;*.mov;*.wav;*.mp3;*.aac;*.m4a"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);
                if (fileDlg.ShowModal() == wxID_OK)
                {
                    path = fileDlg.GetPath();
                }
            }
            else // 目录选择
            {
                wxDirDialog dirDlg(this,
                                 "选择目录",
                                 "",
                                 wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
                if (dirDlg.ShowModal() == wxID_OK)
                {
                    path = dirDlg.GetPath();
                }
            }

            // 更新文本控件
            if (!path.IsEmpty())
            {
                m_dirPathTextCtrl3wx->SetValue(path);
            }
        }
    }

    void MyFrame2::OnToggleButtonClick(wxCommandEvent& event)
    {
        wxBitmapToggleButton* clickedButton = dynamic_cast<wxBitmapToggleButton*>(event.GetEventObject());

        if (clickedButton->GetValue()) {
            clickedButton->SetBitmapLabel(bitmapOn);
        } else {
            clickedButton->SetBitmapLabel(bitmapOff);
        }
    }


