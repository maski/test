#include "wxtest.h"

bool MyApp::OnInit() {
    wxFrame *frame = new MyFrame(wxT("ラバーバンド"));

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
    
#if wxUSE_STATUSBAR
    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to wxWidgets!"));
#endif
    
    m_textureBackground = true;
    m_xAxisReversed = false;
    m_yAxisReversed = false;
    m_mapMode = wxMM_TEXT;
    m_backgroundMode = wxSOLID;
    m_xLogicalOrigin = 0;
    m_yLogicalOrigin = 0;
    m_xUserScale = 1.0;
    m_yUserScale = 1.0;
    
    m_colourForeground = *wxRED;
    m_colourBackground = *wxBLUE;
    
    m_canvas = new MyCanvas(this);
    m_canvas->SetScrollbars(10, 10, 100, 100);
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
    m_clip = false;
#if wxUSE_GRAPHICS_CONTEXT
    m_useContext = false;
#endif
}

void MyCanvas::DrawSplines(wxDC& dc) {
#if wxUSE_SPLINES
    dc.DrawText(wxT("Some splines"), 10, 5);

    const int R = 100;
    const wxPoint center(R + 20, R + 20);
    const int angles[7] = {0, 10, 33, 77, 13, 145, 90};
    const int radii[5] = {100, 59, 85, 33, 90};
    const int n = 200;
    wxPoint pts[n];

    // background spline calculation
    unsigned int radius_pos = 0;
    unsigned int angle_pos = 0;
    int angle = 0;
    for (int i = 0; i < n; i++) {
        angle += angles[angle_pos];
        int r = R * radii[radius_pos] / 100;
        pts[i].x = center.x + (wxCoord)(r * cos( M_PI * angle / 180.0));
        pts[i].y = center.y + (wxCoord)(r * sin( M_PI * angle / 180.0));

        angle_pos++;
        if (angle_pos >= WXSIZEOF(angles)) angle_pos = 0;

        radius_pos++;
        if (radius_pos >= WXSIZEOF(radii)) radius_pos = 0;
    }

    // background spline drawing
    dc.SetPen(*wxRED_PEN);
    dc.DrawSpline(WXSIZEOF(pts), pts);

#else
    dc.DrawText(wxT("Splines not supported."), 10, 5);
#endif
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC pdc(this);

#if wxUSE_GRAPHICS_CONTEXT
    wxGCDC gdc(pdc);
    wxDC &dc = m_useContext ? (wxDC&)gdc : (wxDC&)pdc;
#else
    wxDC &dc = pdc;
#endif

    PrepareDC(dc);
    m_owner->PrepareDC(dc);

    dc.SetBackgroundMode(m_owner->m_backgroundMode);
    if (m_owner->m_backgroundBrush.Ok())
        dc.SetBackground(m_owner->m_backgroundBrush);
    if (m_owner->m_colourForeground.Ok())
        dc.SetTextForeground(m_owner->m_colourForeground);
    if (m_owner->m_colourBackground.Ok())
        dc.SetTextBackground(m_owner->m_colourBackground);

    if (m_owner->m_textureBackground) {
        if (!m_owner->m_backgroundBrush.Ok()) {
            wxColour clr(0, 128, 0);
            wxBrush b(clr, wxSOLID);
            dc.SetBackground(b);
        }
    }

    if (m_clip) {
        dc.SetClippingRegion(100, 100, 100, 100);
    }
    
    dc.Clear();

    if (m_owner->m_textureBackground) {
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        for (int i = 0; i < 200; i++) {
            dc.DrawLine(0, i*10, i*10, 0);
        }
    }

    // DrawSplines(dc);
}

void MyCanvas::OnMouseMotion(wxMouseEvent &event) {
#if wxUSE_STATUSBAR
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);

    wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX(pos.x);
    long y = dc.DeviceToLogicalY(pos.y);
    wxString str;
    str.Printf(wxT("Current mouse position: %d,%d"), (int)x, (int)y);
    m_owner->SetStatusText(str);
#else
    wxUnusedVar(event);
#endif
}

void MyCanvas::OnMouseLeftDown(wxMouseEvent& event) {
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);
    dc.DrawText(wxT("Left button is clicked."), 10, 5);
}

