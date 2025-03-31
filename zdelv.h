#ifndef MYAPP_H
#define MYAPP_H

#include <wx/wx.h>
#include <wx/snglinst.h>
#include "MyFrame.h"
#include "MyFrame2.h"
#include "MyTaskBarIcon.h"


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    wxFrame* GetMainFrame() { return m_frame ? m_frame : nullptr; }
    wxFrame* GetMainFrame2() { return m_frame2 ? m_frame2 : nullptr; }
    wxPanel* Getpanel() { return panel ? panel : nullptr; }
    wxPanel* Getpanel2() { return panel2 ? panel2 : nullptr; }

private:
    wxSingleInstanceChecker* m_checker;
    MyFrame* m_frame = nullptr;
    MyFrame2* m_frame2 = nullptr;
    MyTaskBarIcon* m_taskBarIcon = nullptr;
};
DECLARE_APP(MyApp);
#endif // MYAPP_H


//wxIMPLEMENT_APP(MyApp);