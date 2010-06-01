#ifndef __DOCVIEWSAMPLEH__
#define __DOCVIEWSAMPLEH__

#include "wx/docview.h"

class wxDocManager;

// Define a new application
class MyApp: public wxApp
{
public:
    MyApp();
    bool OnInit();
    int OnExit();
    
    
protected:
    wxDocManager* m_docManager;
};

DECLARE_APP(MyApp)

// Define a new frame
class MyCanvas;
class MyFrame: public wxDocParentFrame
{
    DECLARE_CLASS(MyFrame)
public:
    wxMenu *editMenu;
    
    // This pointer only needed if in single window mode
    MyCanvas *canvas;
    
    MyFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
        const long type);
    
    void OnAbout(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE()
};

extern MyFrame *GetMainFrame();

#define DOCVIEW_CUT     1
#define DOCVIEW_ABOUT   wxID_ABOUT

#endif
