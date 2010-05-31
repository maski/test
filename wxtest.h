#ifndef WXTEST_H
#define WXTEST_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

enum {
    Menu_File_About = 100,
    Menu_File_Quit,
    DOODLE_CUT,
    DOODLE_ADD
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
    
    bool m_xAxisReversed;
    bool m_yAxisReversed;
    int m_backgroundMode;
    int m_mapMode;
    int m_xLogicalOrigin;
    int m_yLogicalOrigin;
    double m_xUserScale;
    double m_yUserScale;
    
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
    
    virtual void OnDraw(wxDC& dc);
    
    void OnPaint(wxPaintEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    
    void OnMouseEvent(wxMouseEvent& event);
    
protected:
    
private:
    bool clicked;
    
    MyFrame* m_owner;
    wxPoint prePos;
    
    DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT(MyCanvas::OnPaint)
    EVT_MOTION(MyCanvas::OnMouseMotion)
    EVT_LEFT_DOWN(MyCanvas::OnMouseLeftDown)
    EVT_MOUSE_EVENTS(MyCanvas::OnMouseEvent)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

#endif

