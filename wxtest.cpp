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
    
    m_canvas = new MyCanvas(this);
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



MyCanvas::MyCanvas(MyFrame* parent) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
    wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
    m_owner = parent;
    clicked = false;
}

void MyCanvas::OnDraw(wxDC& dc) {
    // if (m_view) m_view->OnDraw(&dc);
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
    
    wxPoint pos = event.GetPosition();
    
    if (clicked) {
        dc.DrawLine(prePos.x, prePos.y, pos.x, pos.y);
    }
    clicked = true;
    prePos = pos;
}

void MyCanvas::OnMouseEvent(wxMouseEvent& event) {
    wxClientDC dc(this);
    PrepareDC(dc);
    dc.SetPen(*wxBLACK_PEN);
    
}

