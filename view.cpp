#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "docview.h"
#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS(DrawingView, wxView)

// For drawing lines in a canvas
float xpos = -1.0;
float ypos = -1.0;
float xclicked = -1.0;
float yclicked = -1.0;

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool DrawingView::OnCreate(wxDocument *doc, long WXUNUSED(flags)) {
        frame = GetMainFrame();
        canvas = GetMainFrame()->canvas;
        canvas->view = this;
        
        // Associate the appropriate frame with this view.
        SetFrame(frame);
        
        // Make sure the document manager knows that this is the
        // current view.
        Activate(true);
        
        // Initialize the edit menu Undo and Redo items
        doc->GetCommandProcessor()->SetEditMenu(((MyFrame *)frame)->editMenu);
        doc->GetCommandProcessor()->Initialize();
    
    return true;
}

// プレビューとして一時的に描画
void DrawingView::OnDraw(wxDC *dc) {
    dc->SetFont(*wxNORMAL_FONT);
    dc->SetPen(*wxBLACK_PEN);
    
    wxList::compatibility_iterator node = ((DrawingDocument *)GetDocument())->GetDrawSegments().GetFirst();
    while (node) {
        DrawSegment *seg = (DrawSegment *)node->GetData();
        seg->Draw(dc);
        node = node->GetNext();
    }
}

void DrawingView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint)) {
    if (canvas) canvas->Refresh();
}

// Clean up windows used for displaying the view.
bool DrawingView::OnClose(bool deleteWindow) {
    if (!GetDocument()->Close()) return false;
    
    // Clear the canvas in  case we're in single-window mode,
    // and the canvas stays.
    canvas->ClearBackground();
    canvas->view = (wxView *)NULL;
    canvas = (MyCanvas *)NULL;
    
    wxString s(wxTheApp->GetAppName());
    if (frame) frame->SetTitle(s);
    
    SetFrame((wxFrame *)NULL);
    
    Activate(false);
    
    if (deleteWindow) {
        delete frame;
        return true;
    }
    return true;
}

/*
* Window implementations
*/

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_MOUSE_EVENTS(MyCanvas::OnMouseEvent)
    EVT_MOTION(MyCanvas::OnMouseMotion)
END_EVENT_TABLE()

// Define a constructor for my canvas
MyCanvas::MyCanvas(wxView *v, wxFrame *frame, const wxPoint& pos, const wxSize& size, const long style):
    wxScrolledWindow(frame, wxID_ANY, pos, size, style)
{
    isInitClicked = false;
    view = v;
}

// Define the repainting behaviour
void MyCanvas::OnDraw(wxDC& dc) {
    if (view) view->OnDraw(&dc);
}

// 線のリストを返す
DrawLine *MyCanvas::getDrawLine(long x1, long y1, long x2, long y2) {
    DrawLine *line = new DrawLine;
    line->x1 = x1; 
    line->y1 = y1;
    line->x2 = x2; 
    line->y2 = y2;
    return line;
}

// This implements a tiny doodling program. Drag the mouse using
// the left button.
void MyCanvas::OnMouseEvent(wxMouseEvent& event) {
    if (!view) return;
    
    static DrawSegment *currentSegment = (DrawSegment *)NULL;
    static DrawingDocument *selectDocument = (DrawingDocument *)NULL;
    
    wxClientDC dc(this);
    PrepareDC(dc);
    
    dc.SetPen(*wxBLACK_PEN);
    
    wxPoint pt(event.GetLogicalPosition(dc));
    
    DrawingDocument *doc = (DrawingDocument *)view->GetDocument();
    wxString str;
    str.Printf(wxS("オブジェクトの数：%d"), doc->GetDrawSegments().GetCount());
    
    // 前回の座標の保存
    if (xpos < 0 || ypos < 0) {
        xpos = pt.x;
        ypos = pt.y;
        return;
    }
    
    // 選択されたオブジェクトを削除
    if (deleteMode && event.LeftDown()) {
        if (selectDocument->GetDrawSegments().GetCount() > 0) {
            DrawingDocument *doc = (DrawingDocument *)view->GetDocument();
            doc->GetCommandProcessor()->Submit(new
                DrawingCommand(wxS("オブジェクトの消去"), DRAW_DELETE, doc,
                (DrawSegment *)&selectDocument->GetDrawSegments()));
            selectDocument = (DrawingDocument *)NULL;
        }
    }
    
    // オブジェクト選択
    if (selectMode && event.LeftDown()) {
        // 矩形が表示されている2回目のクリック
        if (isInitClicked) {
            isInitClicked = false;
            wxRect rect(xclicked, yclicked, pt.x, pt.y);
            DrawingDocument *doc = (DrawingDocument *)view->GetDocument();
            for (int i = 0; i < doc->GetDrawSegments().GetCount(); i++) {
                bool all = true;
                wxList::compatibility_iterator node = doc->GetDrawSegments().Item(i);
                while (node) {
                    DrawLine *line = (DrawLine *)node->GetData();
                    
                    str.Printf(wxS("i:%d x:%d y:%d"), i, pt.x, pt.y);
                    dc.DrawText(str, 250, i*20);
                    if (rect.Contains(line->x1, line->y1) && rect.Contains(line->x2, line->y2)) {
                        node = node->GetNext();
                    } else {
                        all = false;
                        break;
                    }
                }
                
                if (all) {
                    wxString str;
                    str.Printf(wxS("i:%d"), i);
                    dc.DrawText(str, 250, i*20);
                    selectDocument->GetDrawSegments().Append((DrawSegment *)doc->GetDrawSegments().Item(i));
                    doc->Modify(true);
                    doc->UpdateAllViews();
                }
            }
        } else {
            selectDocument = (DrawingDocument *)NULL;
            isInitClicked = true;
            xclicked = pt.x;
            yclicked = pt.y;
        }
    }
    
    // オブジェクト選択のキャンセル
    if (selectMode && event.RightDown()) {
        selectDocument = (DrawingDocument *)NULL;
        isInitClicked = false;
        dc.Clear();
        OnDraw(dc);
    }
    
    
    // フリーライン作図とラバーバンド作図が終了条件
    if (currentSegment && (freeLineMode && event.LeftUp()) ||
        (rubberBandMode && event.RightDown()))
    {
        isInitClicked = false;
        
        if (currentSegment->lines.GetCount() == 0) {
            delete currentSegment;
            currentSegment = (DrawSegment *)NULL;
        } else {
            // We've got a valid segment on mouse left up, so store it.
            DrawingDocument *doc = (DrawingDocument *)view->GetDocument();
            
            doc->GetCommandProcessor()->Submit(new DrawingCommand(
                rubberBandMode ? wxS("ラバーバンド") : wxS("フリーライン"), DRAW_ADD, doc, currentSegment));
            
            view->GetDocument()->Modify(true);
            currentSegment = (DrawSegment *)NULL;
        }
    }
    
    // フリーライン作図のドラッグ中
    if (event.Dragging() && freeLineMode) {
        if (!currentSegment) currentSegment = new DrawSegment;
        
        currentSegment->lines.Append(getDrawLine((long)xpos, (long)ypos,
            pt.x, pt.y));
        dc.DrawLine((long)xpos, (long)ypos, pt.x, pt.y);
    }
    
    // ラバーバンド作図の場合
    if (rubberBandMode) {
        if (!currentSegment) currentSegment = new DrawSegment;
        
        if (event.LeftDown()) {
            if (isInitClicked) {
                currentSegment->lines.Append(getDrawLine((long)xclicked,
                    (long)yclicked, pt.x, pt.y));
            }
            
            isInitClicked = true;
            xclicked = pt.x;
            yclicked = pt.y;
        }
    }
    
    // オブジェクト選択かラバーバンドで左クリック押された場合
    if (isInitClicked) {
        dc.Clear();
        if (selectMode) {
            // ドット線にする
            dc.SetPen(*wxBLACK_DASHED_PEN);
            dc.DrawRectangle(xclicked, yclicked, pt.x - xclicked, pt.y - yclicked);
            dc.SetPen(*wxBLACK_PEN);
        }
        
        if (rubberBandMode) {
            // ラバーバンドの描いている途中の線を表示
            currentSegment->Draw(&dc);
            dc.DrawLine(xclicked, yclicked, pt.x, pt.y);
        }
        OnDraw(dc);
    }
    
    dc.DrawText(str, 0, 0);
    
    xpos = pt.x;
    ypos = pt.y;
}

void MyCanvas::OnMouseMotion(wxMouseEvent& event) {
    MyFrame *frame = (MyFrame *)wxTheApp->GetTopWindow();
    wxClientDC dc(this);
    PrepareDC(dc);
    frame->PrepareDC(dc);
    
    wxPoint pt(event.GetLogicalPosition(dc));
    long x = dc.DeviceToLogicalX(pt.x);
    long y = dc.DeviceToLogicalY(pt.y);
    wxString str;
    str.Printf(wxS("x:%d y:%d"), (int)x, (int)y);
    frame->SetStatusText(str);
}
