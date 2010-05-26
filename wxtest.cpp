#include "wxtest.h"

bool MyApp::OnInit() {
    wxFrame *frame = new MyFrame(wxT("ラバーバンド"));

    frame->Show(TRUE);
    SetTopWindow(frame);

    return TRUE;
}



MyFrame::MyFrame(const wxString& title)
    : wxFrame((wxFrame *)NULL, -1, title)
{
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
    
    m_textureBackground = false;
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
    //m_canvas->SetScrollbars(10, 10, 100, 240);
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

void MyCanvas::DrawSplines(wxDC& dc)
{
#if wxUSE_SPLINES
    dc.DrawText(wxT("Some splines"), 10, 5);

    // values are hardcoded rather than randomly generated
    // so the output can be compared between native
    // implementations on platforms with different random
    // generators

    const int R = 300;
    const wxPoint center( R + 20, R + 20 );
    const int angles[7] = { 0, 10, 33, 77, 13, 145, 90 };
    const int radii[5] = { 100 , 59, 85, 33, 90 };
    const int n = 200;
    wxPoint pts[n];

    // background spline calculation
    unsigned int radius_pos = 0;
    unsigned int angle_pos = 0;
    int angle = 0;
    for ( int i = 0; i < n; i++ )
    {
        angle += angles[ angle_pos ];
        int r = R * radii[ radius_pos ] / 100;
        pts[ i ].x = center.x + (wxCoord)( r * cos( M_PI * angle / 180.0) );
        pts[ i ].y = center.y + (wxCoord)( r * sin( M_PI * angle / 180.0) );

        angle_pos++;
        if ( angle_pos >= WXSIZEOF(angles) ) angle_pos = 0;

        radius_pos++;
        if ( radius_pos >= WXSIZEOF(radii) ) radius_pos = 0;
    }

    // background spline drawing
    dc.SetPen(*wxRED_PEN);
    dc.DrawSpline(WXSIZEOF(pts), pts);

    // less detailed spline calculation
    wxPoint letters[4][5];
    // w
    letters[0][0] = wxPoint( 0,1); //  O           O
    letters[0][1] = wxPoint( 1,3); //   *         *
    letters[0][2] = wxPoint( 2,2); //    *   O   *
    letters[0][3] = wxPoint( 3,3); //     * * * *
    letters[0][4] = wxPoint( 4,1); //      O   O

    const int dx = 2 * R / letters[3][4].x;
    const int h[4] = { -R/2, 0, R/4, R/2 };

    for ( int m = 0; m < 1; m++ )
    {
        for ( int n = 0; n < 5; n++ )
        {
            letters[m][n].x = center.x - R + letters[m][n].x * dx;
            letters[m][n].y = center.y + h[ letters[m][n].y ];
        }

        dc.SetPen(wxPen(wxT("blue"), 1, wxDOT));
        dc.DrawLines(5, letters[m]);
        dc.SetPen(wxPen(wxT("black"), 4, wxSOLID));
        // dc.DrawSpline(5, letters[m]);
    }

#else
    dc.DrawText(_T("Splines not supported."), 10, 5);
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

    DrawSplines(dc);
    
}
