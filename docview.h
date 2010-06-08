#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "wx/docview.h"

class wxDocManager;

// Define a new application
class MyApp: public wxApp {
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
class MyFrame: public wxDocParentFrame {
    DECLARE_CLASS(MyFrame)
public:
    wxMenu *editMenu;
    
    // This pointer only needed if in single window mode
    MyCanvas *canvas;
    
    MyFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
        const long type);
    
    void setFlagsAll(bool flags);
    void OnAbout(wxCommandEvent& event);
    
    void OnQuit(wxCommandEvent& event);
    
    void OnFreeLine(wxCommandEvent& event);
    void OnRubberBand(wxCommandEvent& event);
    void OnSelect(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnScale(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE()
};

extern MyFrame *GetMainFrame();

enum {
    DRAWTOOL_CUT,
    DRAWTOOL_QUIT,
    DRAWTOOL_ABOUT,
    DRAWTOOL_FREELINE,
    DRAWTOOL_RUBBERBAND,
    DRAWTOOL_SELECT,
    DRAWTOOL_DELETE,
    DRAWTOOL_SCALE
};

#endif
