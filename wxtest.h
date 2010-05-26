#ifndef WXTEST_H
#define WXTEST_H

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

class MyCanvas;

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void PrepareDC(wxDC& dc);
    
    bool m_textureBackground;
    bool m_xAxisReversed;
    bool m_yAxisReversed;
    int m_backgroundMode;
    int m_mapMode;
    int m_xLogicalOrigin;
    int m_yLogicalOrigin;
    double m_xUserScale;
    double m_yUserScale;
    
    wxColour m_colourForeground;
    wxColour m_colourBackground;
    wxBrush m_backgroundBrush;
    MyCanvas *m_canvas;
    
private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Menu_File_About, MyFrame::OnAbout)
    EVT_MENU(Menu_File_Quit, MyFrame::OnQuit)
END_EVENT_TABLE()

class MyCanvas : public wxScrolledWindow {
public:
    MyCanvas(MyFrame* parent);
    
    void OnPaint(wxPaintEvent& event);
    // void OnMouseMove(wxMouseEvent& event);
    
protected:
    void DrawSplines(wxDC& dc);
private:
    MyFrame* m_owner;
    
    bool m_clip;
#if wxUSE_GRAPHICS_CONTEXT
    bool m_useContext;
#endif
    
    DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

#endif

