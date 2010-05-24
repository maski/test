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
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(wxT("ラバーバンド\n"),
        wxT("このアプリケーションについて"),
        wxOK | wxICON_INFORMATION, this);
}

