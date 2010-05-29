#ifndef WXTEST_H
#define WXTEST_H

#include "wx/wxprec.h"
#include "wx/docview.h"

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

class DrawingView;

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
    DrawingView *m_view;
    
private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Menu_File_About, MyFrame::OnAbout)
    EVT_MENU(Menu_File_Quit, MyFrame::OnQuit)
END_EVENT_TABLE()

class MyCanvas : public wxScrolledWindow {
public:
    MyCanvas(MyFrame* parent, DrawingView* view);
    
    virtual void OnDraw(wxDC& dc);
    
    void OnPaint(wxPaintEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    
    void OnMouseEvent(wxMouseEvent& event);
    
protected:
    
private:
    MyFrame* m_owner;
    DrawingView* m_view;
    
    DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT(MyCanvas::OnPaint)
    EVT_MOTION(MyCanvas::OnMouseMotion)
    EVT_LEFT_DOWN(MyCanvas::OnMouseLeftDown)
    EVT_MOUSE_EVENTS(MyCanvas::OnMouseEvent)
END_EVENT_TABLE()


class DrawingView : public wxView {
public:
    MyFrame *frame;
    MyCanvas *canvas;
    
    DrawingView();
    ~DrawingView();
    
    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *)NULL);
    bool OnClose(bool deleteWindow = true);
    
    void OnCut(wxCommandEvent& event);
    
private:
    DECLARE_DYNAMIC_CLASS(DrawingView)
    DECLARE_EVENT_TABLE()
};

IMPLEMENT_DYNAMIC_CLASS(DrawingView, wxView)

BEGIN_EVENT_TABLE(DrawingView, wxView)
    EVT_MENU(DOODLE_CUT, DrawingView::OnCut)
END_EVENT_TABLE()


IMPLEMENT_APP(MyApp)

#endif

