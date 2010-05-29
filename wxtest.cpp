#include "wxtest.h"

bool MyApp::OnInit() {
    wxFrame *frame = new MyFrame(wxT("Drawツール"));
    
    frame->Show(TRUE);
    SetTopWindow(frame);
    
    return TRUE;
}



MyFrame::MyFrame(const wxString& title) : wxFrame((wxFrame *)NULL, -1, title) {
    wxMenu *menuFile = new wxMenu();
    menuFile->Append(Menu_File_About,
        wxT("Menu(&A)...\tCtrl-A"),
        wxT("このアプリについて"));
    menuFile->AppendSeparator();
    menuFile->Append(Menu_File_Quit,
        wxT("終了(&X)\tAlt-X"),
        wxT("アプリケーションを終了"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("ファイル(&F)"));

    SetMenuBar(menuBar);
    
    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to wxWidgets!"));
    
    m_xAxisReversed = false;
    m_yAxisReversed = false;
    m_mapMode = wxMM_TEXT;
    m_backgroundMode = wxSOLID;
    m_xLogicalOrigin = 0;
    m_yLogicalOrigin = 0;
    m_xUserScale = 1.0;
    m_yUserScale = 1.0;
    
    m_canvas = new MyCanvas(this, (DrawingView *)NULL);
}

void MyFrame::PrepareDC(wxDC& dc) {
    dc.SetLogicalOrigin(m_xLogicalOrigin, m_yLogicalOrigin);
    dc.SetAxisOrientation(!m_xAxisReversed, m_yAxisReversed);
    dc.SetUserScale(m_xUserScale, m_yUserScale);
    dc.SetMapMode(m_mapMode);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(wxT("ラバーバンド\n"),
    wxT("このアプリケーションについて"),
    wxOK | wxICON_INFORMATION, this);
}



MyCanvas::MyCanvas(MyFrame* parent, DrawingView* view) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
    wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
    m_owner = parent;
    m_view = view;
}

void MyCanvas::OnDraw(wxDC& dc) {
    if (m_view) m_view->OnDraw(&dc);
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event)) {
    wxPaintDC pdc(this);
    wxDC &dc = pdc;
    PrepareDC(dc);
    m_owner->PrepareDC(dc);

    dc.SetBackgroundMode(m_owner->m_backgroundMode);
    dc.Clear();
}

void MyCanvas::OnMouseMotion(wxMouseEvent &event) {
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);

    wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX(pos.x);
    long y = dc.DeviceToLogicalY(pos.y);
    wxString str;
    str.Printf(wxT("Current mouse position: %d,%d"), (int)x, (int)y);
    m_owner->SetStatusText(str);
}

void MyCanvas::OnMouseLeftDown(wxMouseEvent& event) {
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);
    dc.DrawText(wxT("Left button is clicked."), 10, 5);
}

void MyCanvas::OnMouseEvent(wxMouseEvent& event) {
    wxClientDC dc(this);
    PrepareDC(dc);
    dc.SetPen(*wxBLACK_PEN);
    
    wxPoint pt(event.GetLogicalPosition(dc));
    
    // m_viewが作られてない。
    //m_view->GetDocument()->Modify(true);
    
    dc.DrawLine(0, 0, pt.x, pt.y);
}

DrawingView::DrawingView() {canvas = (MyCanvas *)NULL;}

DrawingView::~DrawingView() {}

bool DrawingView::OnCreate(wxDocument *doc, long WXUNUSED(flags)) {
    frame = (MyFrame *)NULL;
    frame->Show(true);
    Activate(true);
    
    return true;
}

void DrawingView::OnDraw(wxDC *dc) {
    dc->SetFont(*wxNORMAL_FONT);
    dc->SetPen(*wxBLACK_PEN);
}

void DrawingView::OnUpdate(wxView *sender, wxObject *hint) {
    ;
}

bool DrawingView::OnClose(bool deleteWindow) {
    return true;
}

void DrawingView::OnCut(wxCommandEvent& WXUNUSED(event)) {
    wxDocument *doc = (wxDocument *)GetDocument();
}

