#ifndef VIEW_H
#define VIEW_H

#include "wx/docview.h"

class MyCanvas: public wxScrolledWindow {
public:
    bool isInitClicked;
    bool freeLineMode;
    bool rubberBandMode;
    bool selectMode;
    bool deleteMode;
    bool scaleMode;
    
    wxView *view;
    
    MyCanvas(wxView *v, wxFrame *frame, const wxPoint& pos, const wxSize& size, const long style);
    virtual void OnDraw(wxDC& dc);
    
    // ここで描画する。
    void OnMouseEvent(wxMouseEvent& event);
    
    // マウスが動いたら実行
    void OnMouseMotion(wxMouseEvent &event);
    
private:
    DrawLine *getDrawLine(long x1, long y1, long x2, long y2);
    
    DECLARE_EVENT_TABLE()
};

class DrawingView : public wxView {
    DECLARE_DYNAMIC_CLASS(DrawingView)
public:
    wxFrame *frame;
    MyCanvas *canvas;
    
    DrawingView() {canvas = (MyCanvas *)NULL; frame = (wxFrame *)NULL;};
    ~DrawingView() {};
    
    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *)NULL);
    bool OnClose(bool deleteWindow = true);
private:
};

#endif
