#include "MyFrame.h"
#include "AutoCloseDialog.h"
#include "Command.h"
#include <wx/stdpaths.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/regex.h>
#include <filesystem>

#include "pic/yybj.h"
#include "pic/yybj2.h"
#include "pic/bubj.h"
#include "pic/bubj2.h"
#include "pic/imageon.h"
#include "pic/imageoff.h"
#include "pic/imageon1.h"
#include "pic/imageoff1.h"

//// 外部资源声明（假设这些已在其他资源文件中定义）
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

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_EXIT, MyFrame::OnExit)
    EVT_MENU(ID_HELP, MyFrame::OnHelp)
    EVT_MENU(ID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID_SHOW_HIDE, MyFrame::OnShowHide)
    EVT_BUTTON(wxID_ANY, MyFrame::OnButtonClick)
    EVT_BUTTON(wxID_ANY, MyFrame::KillProcess)
    EVT_BUTTON(wxID_ANY, MyFrame::OnSelectPathButtonClick)
    EVT_TIMER(wxID_ANY, MyFrame::OnTimer5)
    EVT_CLOSE(MyFrame::OnClose)
wxEND_EVENT_TABLE()

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, wxString::FromUTF8("熊猫剪切v1.0.0[去除片头|片尾]"),
      wxDefaultPosition, wxSize(800, 360),
      wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
      process(nullptr), m_timer5(this)
{
    //isDarkMode = IsDarkModeEnabled();
    isDarkMode = Command::IsDarkMode();
    Command::CenterWindow(this);
    // 创建一个菜单
    //void OnClosewindowClicked(wxCloseEvent& event);
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
    //menuBar->Remove(0);

    // 将菜单栏设置到窗口上
    //SetMenuBar(NULL);
    SetMenuBar(menuBar);

    // 绑定菜单事件
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, ID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnHelp, this, ID_HELP);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, ID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnShowHide, this, ID_SHOW_HIDE);

    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* mainSizerall = new wxBoxSizer(wxVERTICAL);
    //inputSizer = new wxBoxSizer(wxHORIZONTAL);
    //buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    controlSizer = new wxBoxSizer(wxHORIZONTAL);
    selectSizer = new wxBoxSizer(wxHORIZONTAL);
    selectSizer2 = new wxBoxSizer(wxHORIZONTAL);
    //adSizer = new wxBoxSizer(wxHORIZONTAL);
    inputSizer2 = new wxBoxSizer(wxHORIZONTAL);
    //checkvideoSizer = new wxBoxSizer(wxHORIZONTAL);
    //checkvideoSizer2 = new wxBoxSizer(wxHORIZONTAL);
    m_panelSelectSizer2 = new wxPanel(panel, wxID_ANY); // 创建容器面板

    //backgroundColor = isDarkMode ? wxColour(36, 36, 36) : wxColour(242, 242, 242);
    backgroundColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    //textColor = isDarkMode ? wxColour(242, 242, 242) : wxColour(36, 36, 36);
    textColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    //customColor = isDarkMode ? wxColour(128, 128, 128) : wxColour(230, 230, 230);
    customColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    customColor2 = isDarkMode ? wxColour(168, 168, 168) : wxColour(242, 242, 242);

    this->SetBackgroundColour(backgroundColor);

    panel->SetBackgroundColour(backgroundColor);

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

    //m_gauge = new CustomGauge(panel, wxID_ANY, 100, wxPoint(20, 20), wxSize(250, 20));
    //m_gauge->SetBackgroundAndForeground(*wxBLACK, *wxGREEN);

    m_dirPathTextCtrl3wx = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_dirPathTextCtrl3wx->SetMinSize(wxSize(640, 30));
    m_dirPathTextCtrl3wx->SetForegroundColour(textColor);
    m_dirPathTextCtrl3wx->SetOwnBackgroundColour(backgroundColor);
    selectDirButton3wx = new CustomButton(panel, wxID_ANY, wxString::FromUTF8("文件路径"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    selectDirButton3wx->SetForegroundColour(textColor);
    selectDirButton3wx->SetOwnBackgroundColour(backgroundColor);

    wxStaticText* staticTexte = new wxStaticText(m_panelSelectSizer2, wxID_ANY, wxString::FromUTF8("添加片头字幕"));
    staticTexte->SetMinSize(wxSize(80, 30));
    staticTexte->SetForegroundColour(textColor);
    m_dirPathTextCtrl3 = new wxTextCtrl(m_panelSelectSizer2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_dirPathTextCtrl3->SetMinSize(wxSize(200, 30));
    m_dirPathTextCtrl3->SetForegroundColour(textColor);
    m_dirPathTextCtrl3->SetOwnBackgroundColour(backgroundColor);
    //selectDirButton3 = new wxButton(panel, wxID_ANY, wxString::FromUTF8("片头视频"));
    selectDirButton3 = new CustomButton(m_panelSelectSizer2, wxID_ANY, wxString::FromUTF8("片头视频"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    selectDirButton3->SetMinSize(wxSize(120, 30));
    selectDirButton3->SetForegroundColour(textColor);
    selectDirButton3->SetOwnBackgroundColour(backgroundColor);
    selectDirButton3->Enable(false);

    m_dirPathTextCtrl4 = new wxTextCtrl(m_panelSelectSizer2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_dirPathTextCtrl4->SetMinSize(wxSize(200, 30));
    m_dirPathTextCtrl4->SetForegroundColour(textColor);
    m_dirPathTextCtrl4->SetOwnBackgroundColour(backgroundColor);
    //selectDirButton4 = new wxButton(panel, wxID_ANY, wxString::FromUTF8("片头字幕"));
    selectDirButton4 = new CustomButton(m_panelSelectSizer2, wxID_ANY, wxString::FromUTF8("字幕文件"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    selectDirButton4->SetMinSize(wxSize(120, 30));
    selectDirButton4->SetForegroundColour(textColor);
    selectDirButton4->SetOwnBackgroundColour(backgroundColor);
    selectDirButton4->Enable(false);

    wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, wxString::FromUTF8("删除片头时长"));
    staticText->SetMinSize(wxSize(100, 30));
    staticText->SetForegroundColour(textColor);
    m_textCtrl = new wxTextCtrl(panel, wxID_ANY);
    m_textCtrl->SetMinSize(wxSize(200, 30));
    m_textCtrl->SetForegroundColour(textColor);
    m_textCtrl->SetOwnBackgroundColour(backgroundColor);
    m_textCtrl->SetValue("0");
    //m_textCtrl2->Bind(wxEVT_KEY_DOWN, &MyFrame::OnTextCtrlKeyDown, this);

    wxStaticText* staticText2 = new wxStaticText(panel, wxID_ANY, wxString::FromUTF8("删除片尾时长"));
    staticText2->SetMinSize(wxSize(100, 30));
    staticText->SetForegroundColour(textColor);
    m_textCtrl2 = new wxTextCtrl(panel, wxID_ANY);
    m_textCtrl2->SetMinSize(wxSize(200, 30));
    m_textCtrl2->SetForegroundColour(textColor);
    m_textCtrl2->SetOwnBackgroundColour(backgroundColor);
    m_textCtrl2->SetValue("0");

    wxTextValidator validator(wxFILTER_DIGITS);
    m_textCtrl->SetValidator(validator);
    m_textCtrl2->SetValidator(validator);

    //m_outputTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY);
    m_outputTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,wxTE_MULTILINE | wxTE_WORDWRAP | wxTE_READONLY | wxTE_NO_VSCROLL | wxTE_NOHIDESEL);
    m_outputTextCtrl->SetMinSize(wxSize(780, 100));
    m_outputTextCtrl->SetForegroundColour(textColor);
    m_outputTextCtrl->SetOwnBackgroundColour(backgroundColor);

    //startButton = new wxButton(panel, wxID_ANY, wxString::FromUTF8("开始处理"));
    startButton = new CustomButton(panel, wxID_ANY, wxString::FromUTF8("开始处理"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    startButton->SetMinSize(wxSize(120, 30));
    startButton->SetForegroundColour(textColor);
    startButton->SetOwnBackgroundColour(backgroundColor);

    stopButton = new CustomButton(panel, wxID_ANY, wxString::FromUTF8("停止处理"),bitmapbubj2,wxDefaultPosition, wxSize(120, 30));
    stopButton->SetMinSize(wxSize(120, 30));
    stopButton->SetForegroundColour(textColor);
    stopButton->SetOwnBackgroundColour(backgroundColor);

    wxStaticText* staticTexth = new wxStaticText(panel, wxID_ANY, wxString::FromUTF8("H265编码"));
    staticTexth->SetForegroundColour(textColor);
    m_toggleButton8 = new wxBitmapToggleButton(panel, wxID_ANY, bitmapOff, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    m_toggleButton8->SetValue(false);

    m_toggleButton8->SetMinSize(wxSize(30, 30));

    wxStaticText* staticTextb = new wxStaticText(panel, wxID_ANY, wxString::FromUTF8("添加字幕"));
    staticTextb->SetForegroundColour(textColor);
    m_toggleButton2 = new wxBitmapToggleButton(panel, wxID_ANY, bitmapOff, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    m_toggleButton2->SetMinSize(wxSize(30, 30));

    selectSizer->Add(selectDirButton3wx, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer->Add(m_dirPathTextCtrl3wx, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    selectSizer2->Add(staticTexte, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer2->Add(selectDirButton3, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer2->Add(m_dirPathTextCtrl3, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer2->Add(selectDirButton4, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    selectSizer2->Add(m_dirPathTextCtrl4, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    inputSizer2->Add(staticText, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    inputSizer2->Add(m_textCtrl, 1, wxALL | wxEXPAND, 5);

    inputSizer2->Add(staticText2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    inputSizer2->Add(m_textCtrl2, 1, wxALL | wxEXPAND, 5);

    controlSizer->Add(staticTextb, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlSizer->Add(m_toggleButton2, 0, wxALL, 5);
    controlSizer->Add(startButton, 0, wxALL, 5);
    controlSizer->Add(stopButton, 0, wxALL, 5);
    controlSizer->Add(staticTexth, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    controlSizer->Add(m_toggleButton8, 0, wxALL, 5);

    //m_panelSelectSizer2->SetSizer(selectSizer2);
    m_panelSelectSizer2->SetSizerAndFit(selectSizer2);

    mainSizer->Add(selectSizer, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(inputSizer2, 0, wxALL | wxEXPAND, 10);
    //mainSizer->Add(selectSizer2, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(m_panelSelectSizer2, 0, wxALL | wxEXPAND, 10);
    //mainSizer->Add(m_outputTextCtrl, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(m_outputTextCtrl, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(controlSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

    m_panelSelectSizer2->Show(false); // 初始隐藏

    m_toggleButton8->Bind(wxEVT_TOGGLEBUTTON, &MyFrame::OnToggleButtonClick, this);
    m_toggleButton2->Bind(wxEVT_TOGGLEBUTTON, &MyFrame::OnToggleButtonClick3, this);
    Bind(wxEVT_BUTTON, &MyFrame::OnButtonClick, this, startButton->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::KillProcess, this, stopButton->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSelectPathButtonClick, this, selectDirButton3wx->GetId());
    Bind(wxEVT_TIMER, &MyFrame::OnTimer5, this, m_timer5.GetId());
    Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);
    Bind(wxEVT_BUTTON, &MyFrame::OnSelectPtButtonClick, this, selectDirButton3->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSelectZmButtonClick, this, selectDirButton4->GetId());
    //Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClosewindowClicked, this);

    //selectSizer2->Show(false);
    //mainSizerall->Add(panel, 1, wxEXPAND);
    mainSizerall->Add(panel, 0, wxALIGN_CENTER | wxFIXED_MINSIZE, 0);
    panel->SetMinSize(wxSize(800, 360));
    Command::SetColorsAndStylesRecursiveN(this,panel);
    SetSizer(mainSizerall);
    m_panelSelectSizer2->Layout();
    //panel->SetSizer(mainSizer);
    panel->SetSizerAndFit(mainSizer);
    panel->Layout();
    panel->Refresh();
    //GetSizer()->SetSizeHints(this); // 新增
    //Fit(); // 新增
}


    void MyFrame:: OnToggleButtonClick3(wxCommandEvent& event) {
        wxBitmapToggleButton* clickedButton = dynamic_cast<wxBitmapToggleButton*>(event.GetEventObject());
        bool isToggled = clickedButton->GetValue();

        clickedButton->SetBitmapLabel(isToggled ? bitmapOn : bitmapOff);
        m_panelSelectSizer2->Show(isToggled);
        inputSizer2->Show(!isToggled);
        selectDirButton3->Enable(isToggled);
        selectDirButton4->Enable(isToggled);
        //thistitle->SetLabel(isToggled ? "添加片头|字幕" : "去除片头|片尾");
        this->SetTitle(isToggled ? "熊猫剪切v1.0.0[添加片头|字幕]" : "熊猫剪切v1.0.0[去除片头|片尾]");
        //panel->SetBackgroundColour(backgroundColor);

        // 更新布局并调整窗口尺寸
        m_panelSelectSizer2->Layout();
        //GetSizer()->SetSizeHints(this); // 重新计算尺寸
        m_panelSelectSizer2->GetParent()->Fit(); // 确保父窗口（如wxFrame）调整尺寸
        panel->Layout();
        //GetSizer()->SetSizeHints(this); // 重新计算尺寸
        //panel->GetParent()->Fit(); // 确保父窗口（如wxFrame）调整尺寸
        //panel->GetParent()->Fit(); // 确保父窗口（如wxFrame）调整尺寸
    }

    void MyFrame::OnSelectPtButtonClick(wxCommandEvent& event) {
        wxFileDialog openFileDialog(this, wxString::FromUTF8("选择片头文件"), "", "",
                                    wxString::FromUTF8("媒体文件 (*.mp4;*.mov)|*.mp4;*.mov"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL) {
            return;
        }

        wxString filePath3 = openFileDialog.GetPath();
        //ptfilePath = filePath3;

        m_dirPathTextCtrl3->SetValue(filePath3);

        //wxString homeDir = wxGetHomeDir();
        //wxString filename = !m_dirPath.IsEmpty() ? wxString::Format(m_dirPath + "/pt_zplayer.txt") : wxString::Format(homeDir + "/zsprundir/pt_zplayer.txt");
        ////filename =	Command::NormalizePath(filename.ToStdString());
        //saveToFile(filename.ToStdString(), ptfilePath.ToStdString());
        ////std::cout << "数据已保存到文件: " << filename.ToStdString() << std::endl;
    }

    void MyFrame::OnSelectZmButtonClick(wxCommandEvent& event) {
        wxFileDialog openFileDialog(this, wxString::FromUTF8("选择字幕文件"), "", "",
                                    wxString::FromUTF8("字幕文件 (*.srt;*.ass;*.sub;*.vtt)|*.srt;*.ass;*.sub;*.vtt"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

        if (openFileDialog.ShowModal() == wxID_CANCEL) {
            return;
        }

        wxString filePath4 = openFileDialog.GetPath();
        //zmfilePath = filePath4;

        m_dirPathTextCtrl4->SetValue(filePath4);

        //wxString homeDir = wxGetHomeDir();
        //wxString filename = !m_dirPath.IsEmpty() ? wxString::Format(m_dirPath + "/zm_zplayer.txt") : wxString::Format(homeDir + "/zsprundir/zm_zplayer.txt");
        ////filename =	Command::NormalizePath(filename.ToStdString());
        //saveToFile(filename.ToStdString(), zmfilePath.ToStdString());
        ////std::cout << "数据已保存到文件: " << filename.ToStdString() << std::endl;
    }

    // 菜单事件处理
    void MyFrame::OnHelp(wxCommandEvent& event) {
        wxString homeDir = wxGetHomeDir();
        wxString filePath = homeDir + "/zsprundir/source/b/help.mp4";
        filePath = Command::NormalizePath(filePath.ToStdString());
        Command::PlayVideo(filePath);
    }

    void MyFrame::OnAbout(wxCommandEvent& event) {
        wxLaunchDefaultBrowser(wxString::FromUTF8("https://mv.6deng.cn:8443/mobile/app"));
    }

    void MyFrame::OnExit(wxCommandEvent& event) {
        //wxString cmd;
        //cmd = "sh -c \"ps -ef | grep -i iina | grep -v grep | awk '{print \\\"kill -15 \\\" $2}' | sh\"";
        //wxExecute(cmd, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE);
        wxApp::GetInstance()->ExitMainLoop();
    }

    void MyFrame::OnClose(wxCloseEvent& event) {
        //this->Hide();
        this->Iconize(true);
        this->SetWindowStyle(this->GetWindowStyle() & ~wxSTAY_ON_TOP);
        event.Veto();
    }

    void MyFrame::OnShowHide(wxCommandEvent& event) {
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
    void MyFrame::RunCommandWithOutput(const wxString& command, int refreshTime, const wxString& filepath) {
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

void MyFrame::KillProcess(wxCommandEvent&) {
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
    void MyFrame::OnTimer5(wxTimerEvent&) {
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

    void MyFrame::ReadAndDisplayStream(wxInputStream* stream, const wxString& prefix) {
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
                //m_outputTextCtrl->WriteText(prefix + output);
            }
        }
    }

    wxString MyFrame::RemoveAnsiEscapeCodes(const wxString& input) {
        static wxRegEx ansiEscapeRegex("\\x1B\\[[0-9;]*[mK]");
        wxString cleaned = input;
        ansiEscapeRegex.ReplaceAll(&cleaned, "");
        return cleaned;
    }

    void MyFrame::OnButtonClick(wxCommandEvent&) {
        if (process) {
            //wxMessageBox("程序已经运行", "提示", wxOK | wxICON_INFORMATION);
            AutoCloseDialog* dlg = new AutoCloseDialog(this, wxString::FromUTF8("程序已经运行"), 3000);
            dlg->Show();
            return;
        }
        wxStandardPaths& stdPaths = wxStandardPaths::Get();
        wxString resourceexecutableFilePath = stdPaths.GetResourcesDir() + "/zdvq";
        const std::string pythonPath = "/usr/local/bin/python3";

        wxString filePath0 = m_dirPathTextCtrl3wx->GetValue();
        filePath0 = Command::ClearAllSpace(Command::NormalizePath(filePath0.ToStdString()));
        //wxString filePath;
        if (!fs::exists(filePath0.ToStdString())) {
            wxString message = wxString::Format("路径不存在: %s", filePath0);
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
        wxString command;
        if (m_toggleButton2->GetValue()) {
            //zdvq ad ./1a.mp4 4 1a.vtt a.mp4
            wxString ptdirPath = m_dirPathTextCtrl3->GetValue();
            ptdirPath = Command::NormalizePath(ptdirPath.ToStdString());

            wxString zmdirPath = m_dirPathTextCtrl4->GetValue();
            zmdirPath = Command::NormalizePath(zmdirPath.ToStdString());

            if(ptdirPath.IsEmpty() && zmdirPath.IsEmpty()){
            	wxMessageBox("[警告]:既不加字幕，也不加片头视频，你在玩吗？。", "提示", wxOK | wxICON_INFORMATION);
            	return;
            }

        if (!fs::exists(ptdirPath.ToStdString()) && !ptdirPath.IsEmpty()) {
            wxString message = wxString::Format("路径不存在: %s", ptdirPath);
            wxMessageBox(message, "提示", wxOK | wxICON_INFORMATION);
            return;
        }
            wxString ptDir = !ptdirPath.IsEmpty() ? ptdirPath : "";

        if (!fs::exists(zmdirPath.ToStdString()) && !zmdirPath.IsEmpty()) {
            wxString message = wxString::Format("路径不存在: %s", zmdirPath);
            wxMessageBox(message, "提示", wxOK | wxICON_INFORMATION);
            return;
        }
            wxString zmDir = !zmdirPath.IsEmpty() ? zmdirPath : "";
            command = wxString::Format("%s %s 'ad' %s %d \"%s\" \"%s\"", pythonPath, resourceexecutableFilePath, filePath, h265tag, zmDir, ptDir);
        }else{
            wxString pttime0 = m_textCtrl->GetValue();
            pttime0 = Command::ClearAllSpace(pttime0);
            int pttime = 0;
            if (!pttime0.IsEmpty()) {
                // 确保字符串可以成功转换为整数
                long tempTime = 0;
                if (pttime0.ToLong(&tempTime)) {
                    pttime = static_cast<int>(tempTime);
                }
            }

            wxString pwtime0 = m_textCtrl2->GetValue();
            pwtime0 = Command::ClearAllSpace(pwtime0);
            int pwtime = 0;
            if (!pwtime0.IsEmpty()) {
                // 确保字符串可以成功转换为整数
                long tempTime = 0;
                if (pwtime0.ToLong(&tempTime)) {
                    pwtime = static_cast<int>(tempTime);
                }
            }

            if (pttime == 0 && pwtime ==0) {
                wxMessageBox("[警告]:既不去片头，也不去片尾，你在玩吗？", "提示", wxOK | wxICON_INFORMATION);
                return;
            }
            //wxMessageBox(wxString::Format("%d", h265tag), "提示", wxOK | wxICON_INFORMATION);
            //wxMessageBox(wxString::Format("%s", filePath), "提示", wxOK | wxICON_INFORMATION);
            command = wxString::Format("%s %s 'tw' %s %d %d %d", pythonPath, resourceexecutableFilePath, filePath, pttime, pwtime, h265tag);
            //command = wxString::Format("/bin/bash -c \"%s 'tw' %s %d %d %d\"", resourceexecutableFilePath, filePath, pttime, pwtime, h265tag);
        }
        //wxMessageBox(command, "提示", wxOK | wxICON_INFORMATION);
        int refreshTime = 50;
        RunCommandWithOutput(command, refreshTime,filePath);
    }
    void MyFrame::OnSelectPathButtonClick(wxCommandEvent& event) {
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

    void MyFrame::OnToggleButtonClick(wxCommandEvent& event)
    {
        wxBitmapToggleButton* clickedButton = dynamic_cast<wxBitmapToggleButton*>(event.GetEventObject());

        if (clickedButton->GetValue()) {
            clickedButton->SetBitmapLabel(bitmapOn);
        } else {
            clickedButton->SetBitmapLabel(bitmapOff);
        }
    }

    double ExtractProgressPercentage(const wxString& output) {
        wxRegEx fragRegex("frag (\\d+)/(\\d+)");
        wxRegEx percentRegex("\\[download\\] .*? (\\d+\\.\\d+)%");
        wxRegEx percentRegex2("\\[(\\d+\\.\\d+)%\\]");
        wxRegEx percentRegex3("\\[EmbedThumbnail\\](.*?)");
        wxRegEx percentRegex4("\\[Metadata\\](.*?)");
        wxRegEx percentRegex5("\\[FixupM4a\\](.*?)");
        wxRegEx percentRegex6("\\[ExtractAudio\\](.*?)");
        wxRegEx percentRegex7("\\[Merger\\](.*?)");
        wxRegEx percentRegex8("\\[.*?\\((\\d+)%\\)");
        wxRegEx percentRegex9("\\[.*?\\[(\\d+\\.\\d+)%\\]");
        //[#ab3667 82MiB/2.3GiB(3%) CN:16 DL:3.5MiB ETA:10m51s]

        wxString currentFragStr, totalFragStr;
        wxString percentStr;

        wxString cleanOutput = output;
        wxRegEx ansiEscapeRegex("\\x1B\\[[0-9;]*m");
        while (ansiEscapeRegex.Matches(cleanOutput)) {
            cleanOutput.Replace(ansiEscapeRegex.GetMatch(cleanOutput, 0), wxEmptyString);
        }
        //m_ffmpeging = false;

        try {
            if (fragRegex.Matches(cleanOutput)) {
                currentFragStr = fragRegex.GetMatch(cleanOutput, 1);
                totalFragStr = fragRegex.GetMatch(cleanOutput, 2);

                long currentFrag = wxAtol(currentFragStr);
                long totalFrag = wxAtol(totalFragStr);

                if (totalFrag > 0) {
                    return (static_cast<double>(currentFrag) / totalFrag) * 100.0;
                }
            } else if (percentRegex.Matches(cleanOutput)) {
                percentStr = percentRegex.GetMatch(cleanOutput, 1);
                return std::stod(percentStr.ToStdString());
            } else if (percentRegex8.Matches(cleanOutput)) {
                percentStr = percentRegex8.GetMatch(cleanOutput, 1);
                return std::stod(percentStr.ToStdString());
            } else if (percentRegex9.Matches(cleanOutput)) {
                percentStr = percentRegex9.GetMatch(cleanOutput, 1);
                return std::stod(percentStr.ToStdString());
            } else if (percentRegex2.Matches(cleanOutput)) {
                //m_ffmpeging = true;
                return 90.0;
                //percentStr = percentRegex2.GetMatch(cleanOutput, 1);
                //return std::stod(percentStr.ToStdString());
            } else if (percentRegex3.Matches(cleanOutput) || percentRegex4.Matches(cleanOutput) || percentRegex5.Matches(cleanOutput) || percentRegex6.Matches(cleanOutput) || percentRegex7.Matches(cleanOutput)) {
                //m_ffmpeging = true;
                return 90.0;
            }

        } catch (const std::exception& e) {
            // 捕获异常，返回默认值
            return -1;
        }

        return -1;
    }
