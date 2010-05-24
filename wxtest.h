#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

enum {
    Menu_File_About = 100,
    Menu_File_Quit,
};

class MyApp : public wxApp {
public:
    virtual bool OnInit();
private:
    
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Menu_File_About, MyFrame::OnAbout)
    EVT_MENU(Menu_File_Quit, MyFrame::OnQuit)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

