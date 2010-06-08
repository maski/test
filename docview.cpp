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

MyApp::MyApp() {
    m_docManager = (wxDocManager *)NULL;
}

bool MyApp::OnInit() {
    //// Create a document manager
    m_docManager = new wxDocManager;
    m_docManager->SetMaxDocsOpen(1);
    
    //// Create a template relating drawing documents to their views
    new wxDocTemplate(m_docManager, wxS("Drawツール"), wxS("*.bmp"), wxS(""), wxS(""),
        wxS("Draw"), wxS("Drawing View"),
        CLASSINFO(DrawingDocument), CLASSINFO(DrawingView));
    
    //// Create the main frame window
    frame = new MyFrame(m_docManager, (wxFrame *)NULL, wxID_ANY,
        wxS("Drawツール"), wxPoint(0, 0), wxSize(500, 500),
        wxDEFAULT_FRAME_STYLE | wxFRAME_SHAPED);
    
    //// Make a menubar
    wxMenu *file_menu = new wxMenu;
    wxMenu *edit_menu = (wxMenu *)NULL;
    
    file_menu->Append(wxID_EXIT, wxS("終了(&X)\tAlt+F4"), wxS("プログラムを終了する"));
    
    edit_menu = new wxMenu;
    edit_menu->Append(wxID_UNDO, wxS("元に戻す(&U)"));
    edit_menu->Append(wxID_REDO, wxS("やり直し(&R)"));
    edit_menu->AppendSeparator();
    edit_menu->Append(DRAWTOOL_FREELINE, wxS("フリーライン作図(&F)"), wxS("フリーハンドの線"));
    edit_menu->Append(DRAWTOOL_RUBBERBAND, wxS("ラバーバンド作図(&R)"), wxS("多角形が書ける"));
    edit_menu->Append(DRAWTOOL_SELECT, wxS("オブジェクト選択(&S)"), wxS("描いた線を選択出来る"));
    edit_menu->Append(DRAWTOOL_DELETE, wxS("消去(&D)"), wxS("選択した線を消去"));
    edit_menu->Append(DRAWTOOL_SCALE, wxS("拡大・縮小(&O)"), wxS("選択した線の拡大縮小"));
    frame->editMenu = edit_menu;
    
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(DRAWTOOL_ABOUT, wxS("このソフトについて"), wxS("このソフトについて説明"));
    
    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, wxS("ファイル(&F)"));
    menu_bar->Append(edit_menu, wxS("編集(&E)"));
    menu_bar->Append(help_menu, wxS("ヘルプ(&H)"));
    
    
    
    int width, height;
    frame->GetClientSize(&width, &height);
    
    // Non-retained canvas
    frame->canvas = new MyCanvas((wxView *)NULL, frame, wxPoint(0, 0), wxSize(width, height), 0);
    frame->canvas->SetCursor(wxCursor(wxCURSOR_PENCIL));
    
    frame->canvas->SetBackgroundColour(*wxWHITE);
    frame->canvas->ClearBackground();
    frame->setFlagsAll(false);
    
    
    //// Associate the menu bar with the frame
    frame->SetMenuBar(menu_bar);
    
    frame->CreateStatusBar(1);
    frame->SetStatusText(wxS("Drawツール"));
    
    frame->ProcessCommand(wxID_NEW);
    
    // フレームを中央に移動
    frame->Centre(wxBOTH);
    frame->Show(true);
    
    SetTopWindow(frame);
    
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
    EVT_MENU(DRAWTOOL_QUIT, MyFrame::OnQuit)
    EVT_MENU(DRAWTOOL_ABOUT, MyFrame::OnAbout)
    EVT_MENU(DRAWTOOL_FREELINE, MyFrame::OnFreeLine)
    EVT_MENU(DRAWTOOL_RUBBERBAND, MyFrame::OnRubberBand)
    EVT_MENU(DRAWTOOL_SELECT, MyFrame::OnSelect)
    EVT_MENU(DRAWTOOL_DELETE, MyFrame::OnDelete)
    EVT_MENU(DRAWTOOL_SCALE, MyFrame::OnScale)
END_EVENT_TABLE()

MyFrame::MyFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title,
                 const wxPoint& pos, const wxSize& size, const long type):
wxDocParentFrame(manager, frame, id, title, pos, size, type)
{
    // This pointer only needed if in single window mode
    canvas = (MyCanvas *)NULL;
    editMenu = (wxMenu *)NULL;
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(wxS("wxDocTemplateを\n使っています。"), wxS("Drawツールについて"));
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
}

void MyFrame::setFlagsAll(bool flags) {
    canvas->isInitClicked = flags;
    canvas->freeLineMode = flags;
    canvas->rubberBandMode = flags;
    canvas->selectMode = flags;
    canvas->deleteMode = flags;
    canvas->scaleMode = flags;
}

void MyFrame::OnFreeLine(wxCommandEvent& event) {
    setFlagsAll(false);
    canvas->freeLineMode = true;
}

void MyFrame::OnRubberBand(wxCommandEvent& event) {
    setFlagsAll(false);
    canvas->rubberBandMode = true;
}

void MyFrame::OnSelect(wxCommandEvent& event) {
    setFlagsAll(false);
    canvas->selectMode = true;
    
}

void MyFrame::OnDelete(wxCommandEvent& event) {
    setFlagsAll(false);
    canvas->deleteMode = true;
}

void MyFrame::OnScale(wxCommandEvent& event) {
    setFlagsAll(false);
    canvas->scaleMode = true;
}

MyFrame *GetMainFrame() {
    return frame;
}

