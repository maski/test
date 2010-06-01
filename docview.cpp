// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/docview.h"

#include "docview.h"
#include "doc.h"
#include "view.h"

MyFrame *frame = (MyFrame *)NULL;

IMPLEMENT_APP(MyApp)

MyApp::MyApp()
{
    m_docManager = (wxDocManager *)NULL;
}

bool MyApp::OnInit()
{
    //// Create a document manager
    m_docManager = new wxDocManager;

    //// Create a template relating drawing documents to their views
    new wxDocTemplate(m_docManager, wxT("Drawing"), wxT(""), wxT(""), wxT(""),
        wxT("Draw"), wxT("Drawing View"),
        CLASSINFO(DrawingDocument), CLASSINFO(DrawingView));
    
    //// Create the main frame window
    frame = new MyFrame(m_docManager, (wxFrame *)NULL, wxID_ANY,
        wxT("Drawツール"), wxPoint(0, 0), wxSize(500, 500),
        wxDEFAULT_FRAME_STYLE);
    
    //// Make a menubar
    wxMenu *file_menu = new wxMenu;
    wxMenu *edit_menu = (wxMenu *)NULL;
    
    file_menu->Append(wxID_NEW, wxT("&New..."));
    
    edit_menu = new wxMenu;
    edit_menu->Append(wxID_UNDO, wxT("&Undo"));
    edit_menu->Append(wxID_REDO, wxT("&Redo"));
    edit_menu->AppendSeparator();
    edit_menu->Append(DOCVIEW_CUT, wxT("&Cut last segment"));
    
    frame->editMenu = edit_menu;
    
    
    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT, wxT("E&xit"));
    
    // A nice touch: a history of files visited. Use this menu.
    m_docManager->FileHistoryUseMenu(file_menu);
    
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(DOCVIEW_ABOUT, wxT("&About"));
    
    wxMenuBar *menu_bar = new wxMenuBar;
    
    menu_bar->Append(file_menu, wxT("&File"));
    if (edit_menu)
        menu_bar->Append(edit_menu, wxT("&Edit"));
    menu_bar->Append(help_menu, wxT("&Help"));
    
    
    
    int width, height;
    frame->GetClientSize(&width, &height);
    
    // Non-retained canvas
    frame->canvas = new MyCanvas((wxView *)NULL, frame, wxPoint(0, 0), wxSize(width, height), 0);
    frame->canvas->SetCursor(wxCursor(wxCURSOR_PENCIL));
    
    // Give it scrollbars
    frame->canvas->SetScrollbars(10, 10, 10, 10);
    frame->canvas->SetBackgroundColour(*wxWHITE);
    frame->canvas->ClearBackground();
    
    
    
    //// Associate the menu bar with the frame
    frame->SetMenuBar(menu_bar);
    
    frame->Centre(wxBOTH);
    frame->Show(true);
    
    SetTopWindow(frame);
    
    m_docManager->OnFileNew((wxCommandEvent&)NULL);
    
    return true;
}

int MyApp::OnExit() {
    delete m_docManager;
    return 0;
}

/*
* This is the top-level window of the application.
*/

IMPLEMENT_CLASS(MyFrame, wxDocParentFrame)
BEGIN_EVENT_TABLE(MyFrame, wxDocParentFrame)
    EVT_MENU(DOCVIEW_ABOUT, MyFrame::OnAbout)
END_EVENT_TABLE()

MyFrame::MyFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title,
                 const wxPoint& pos, const wxSize& size, const long type):
wxDocParentFrame(manager, frame, id, title, pos, size, type)
{
    // This pointer only needed if in single window mode
    canvas = (MyCanvas *)NULL;
    editMenu = (wxMenu *)NULL;
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    wxMessageBox(wxT("Drawツール"), wxT("Drawツールについて"));
}

MyFrame *GetMainFrame()
{
    return frame;
}

