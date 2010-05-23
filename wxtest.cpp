#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit() {
    wxFrame *frame = new wxFrame(NULL,
        -1, wxT("ウインドウ\u2621"), wxPoint(50, 50), wxSize(450, 340)
        );
    
    wxString str;

    frame->Show(TRUE);
    SetTopWindow(frame);


    return TRUE;
}

