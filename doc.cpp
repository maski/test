#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS(DrawingDocument, wxDocument)

DrawingDocument::~DrawingDocument() {
    WX_CLEAR_LIST(wxList, drawSegments);
}

DrawSegment::DrawSegment(const DrawSegment& seg) : wxObject() {
    wxList::compatibility_iterator node = seg.lines.GetFirst();
    while (node) {
        DrawLine *line = (DrawLine *)node->GetData();
        DrawLine *newLine = new DrawLine;
        
        if (rect.x < line->x1) rect.x = line->x1;
        if (rect.y < line->y1) rect.x = line->x1;
        if (rect.width < abs(line->x2 - line->x1)) rect.width = abs(line->x2 - line->x1);
        if (rect.height < abs(line->y2 - line->y1)) rect.height = abs(line->y2 - line->y1);
        
        newLine->x1 = line->x1;
        newLine->y1 = line->y1;
        newLine->x2 = line->x2;
        newLine->y2 = line->y2;

        lines.Append(newLine);

        node = node->GetNext();
    }
}

DrawSegment::~DrawSegment() {
    WX_CLEAR_LIST(wxList, lines);
}

void DrawSegment::Draw(wxDC *dc) {
    // マウスがupのとき描写
    wxList::compatibility_iterator node = lines.GetFirst();
    while (node) {
        DrawLine *line = (DrawLine *)node->GetData();
        dc->DrawLine(line->x1, line->y1, line->x2, line->y2);
        node = node->GetNext();
    }
}

/*
* Implementation of drawing command
*/
DrawingCommand::DrawingCommand(const wxString& name, int command,
    DrawingDocument *ddoc, DrawSegment *seg) : wxCommand(true, name)
{
    doc = ddoc;
    segment = seg;
    cmd = command;
}

DrawingCommand::~DrawingCommand() {
    if (segment) delete segment;
}

bool DrawingCommand::Do() {
    switch (cmd) {
        case DRAW_DELETE:
        {
            // Cut the last segment
            if (segment) {
                wxList *list = (wxList *)segment;
                for (int i = 0; i < list->GetCount(); i++) {
                    doc->GetDrawSegments().Erase(list->Item(i));
                }
                delete segment;
            }
            doc->Modify(true);
            doc->UpdateAllViews();
            break;
        }
        
        // 線が追加されたとき
        case DRAW_ADD:
        {   // ドキュメント（線のリストのリスト）にセグメント（線のリスト）を追加
            doc->GetDrawSegments().Append(new DrawSegment(*segment));
            doc->Modify(true);
            doc->UpdateAllViews();
            break;
        }
    }
    return true;
}

bool DrawingCommand::Undo() {
    switch (cmd)
    {
        case DRAW_DELETE:
        {
            doc->GetDrawSegments().Append(new DrawSegment(*segment));
            doc->Modify(true);
            doc->UpdateAllViews();
            break;
        }
        
        // Undoされたとき
        case DRAW_ADD:
        {
            // Cut the last segment
            if (doc->GetDrawSegments().GetCount() > 0) {
                wxList::compatibility_iterator node = doc->GetDrawSegments().GetLast();
                DrawSegment *seg = (DrawSegment *)node->GetData();
                delete seg;
                doc->GetDrawSegments().Erase(node);

                doc->Modify(true);
                doc->UpdateAllViews();
            }
        }
    }
    return true;
}

