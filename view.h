#ifndef __VIEWSAMPLEH__
#define __VIEWSAMPLEH__

#include "wx/docview.h"

class MyCanvas: public wxScrolledWindow {
public:
    wxView *view;
    
    MyCanvas(wxView *v, wxFrame *frame, const wxPoint& pos, const wxSize& size, const long style);
    virtual void OnDraw(wxDC& dc);
    void OnMouseEvent(wxMouseEvent& event);
    
    DECLARE_EVENT_TABLE()
};

class DrawingView: public wxView
{
    DECLARE_DYNAMIC_CLASS(DrawingView)
private:
public:
    wxFrame *frame;
    MyCanvas *canvas;
    
    DrawingView() { canvas = (MyCanvas *)NULL; frame = (wxFrame *)NULL; };
    ~DrawingView() {};
    
    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    bool OnClose(bool deleteWindow = true);
    
    void OnCut(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE()
};

#endif
