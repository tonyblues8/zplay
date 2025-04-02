#include "ThemeListener.h"
#include "Command.h"

//bool isDarkMode = Command::IsDarkMode();
//wxString filePath;
//wxBitmap bitmapbubj;
//wxBitmap bitmapbubj2;
//wxColour backgroundColor;
//wxColour textColor;
//wxColour customColor;
//wxColour customColor2;
//wxBitmap bitmapOff;
//wxBitmap bitmapOn;
//namespace fs = std::filesystem;
wxPanel* panel = nullptr;
wxPanel* panel2 = nullptr;
wxPanel* m_panelSelectSizer2 = nullptr;


wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    //const std::string pythonPath = "/usr/local/bin/python3"; // Homebrew Python 路径
    //Command::SetupEnvironment();
    const std::string CYAN = "\033[1;36m";
    const std::string RESET = "\033[0m";
    const std::string pythonPath = "/usr/local/bin/python3"; // Homebrew Python 路径
    Command::SetupEnvironment();
    Command::CheckPythonInstallation(pythonPath);

    // 检查 Python 和 pip
    //std::string output;
    //if (Command::ExecCommand("command -v /usr/local/bin/python3 > /dev/null 2>&1", &output) == 0 &&
    //    Command::CheckPip(pythonPath) &&
    //    Command::CheckPythonVersion(pythonPath)) {
    //    std::cout << CYAN << "Python 3.10 found!" << RESET << std::endl;
    //} else {
    //    std::cerr << CYAN << "Python 3.10 or pip not found! Please install manually." << RESET << std::endl;
    //    wxMessageBox("Python 3.10 or pip not found! Please install manually.", "提示", wxOK | wxICON_INFORMATION);
    //    return 1;
    //}

    // 检查并安装所需的 Python 包
    std::vector<std::pair<std::string, std::string>> packages = {
        {"faiss", "faiss-cpu"},
        {"moviepy", ""},
        {"pydub", ""},
        {"torch", ""},
        {"transformers", ""},
        {"numpy", ""},
        {"atexit", ""},
        {"librosa", ""}
    };

    for (const auto& pkg : packages) {
        try {
            Command::InstallPythonPackage(pythonPath, pkg.first, pkg.second);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            wxMessageBox(e.what(), "提示", wxOK | wxICON_INFORMATION);
        }
    }

    if (m_frame) {
    	Command cmd;
      if (cmd.CheckRun(m_frame)){
      	return true;
      }
    };

	  wxInitAllImageHandlers();
    m_frame = new MyFrame();
    m_frame2 = new MyFrame2();
    m_frame->Show(true);
    m_frame2->Show(false);
    m_taskBarIcon = new MyTaskBarIcon(m_frame,m_frame2);
    m_frame->SetWindowStyle(m_frame->GetWindowStyle() | wxSTAY_ON_TOP);
    ThemeListener m_themeListener;
    m_themeListener.Register();// 注册主题监听
	  return true;
}

int MyApp::OnExit()
{
	delete m_taskBarIcon;
	delete m_checker;
	delete m_frame;
	delete m_frame2;
	//delete m_taskBarIcon2;
	//wxApp::GetInstance()->ExitMainLoop();
	return wxApp::OnExit();
	//return 0;
}




