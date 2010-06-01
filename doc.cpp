// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if wxUSE_STD_IOSTREAM
    #include "wx/ioswrap.h"
#else
    #include "wx/txtstrm.h"
#endif

#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS(DrawingDocument, wxDocument)

DrawingDocument::~DrawingDocument() {
    WX_CLEAR_LIST(wxList, doodleSegments);
}

DoodleSegment::DoodleSegment(const DoodleSegment& seg):wxObject() {
    wxList::compatibility_iterator node = seg.lines.GetFirst();
    while (node) {
        DoodleLine *line = (DoodleLine *)node->GetData();
        DoodleLine *newLine = new DoodleLine;
        newLine->x1 = line->x1;
        newLine->y1 = line->y1;
        newLine->x2 = line->x2;
        newLine->y2 = line->y2;

        lines.Append(newLine);

        node = node->GetNext();
    }
}

DoodleSegment::~DoodleSegment() {
    WX_CLEAR_LIST(wxList, lines);
}

void DoodleSegment::Draw(wxDC *dc) {
    wxList::compatibility_iterator node = lines.GetFirst();
    while (node) {
        DoodleLine *line = (DoodleLine *)node->GetData();
        dc->DrawLine(line->x1, line->y1, line->x2, line->y2);
        node = node->GetNext();
    }
}

/*
* Implementation of drawing command
*/

DrawingCommand::DrawingCommand(const wxString& name, int command, DrawingDocument *ddoc, DoodleSegment *seg):
wxCommand(true, name) {
    doc = ddoc;
    segment = seg;
    cmd = command;
}

DrawingCommand::~DrawingCommand() {
    if (segment) delete segment;
}

bool DrawingCommand::Do() {
    switch (cmd)
    {
    case DOODLE_CUT:
        {
            // Cut the last segment
            if (doc->GetDoodleSegments().GetCount() > 0)
            {
                wxList::compatibility_iterator node = doc->GetDoodleSegments().GetLast();
                if (segment)
                    delete segment;

                segment = (DoodleSegment *)node->GetData();
                doc->GetDoodleSegments().Erase(node);

                doc->Modify(true);
                doc->UpdateAllViews();
            }
            break;
        }
    case DOODLE_ADD:
        {
            doc->GetDoodleSegments().Append(new DoodleSegment(*segment));
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
    case DOODLE_CUT:
        {
            // Paste the segment
            if (segment)
            {
                doc->GetDoodleSegments().Append(segment);
                doc->Modify(true);
                doc->UpdateAllViews();
                segment = (DoodleSegment *) NULL;
            }
            doc->Modify(true);
            doc->UpdateAllViews();
            break;
        }
    case DOODLE_ADD:
        {
            // Cut the last segment
            if (doc->GetDoodleSegments().GetCount() > 0)
            {
                wxList::compatibility_iterator node = doc->GetDoodleSegments().GetLast();
                DoodleSegment *seg = (DoodleSegment *)node->GetData();
                delete seg;
                doc->GetDoodleSegments().Erase(node);

                doc->Modify(true);
                doc->UpdateAllViews();
            }
        }
    }
    return true;
}

