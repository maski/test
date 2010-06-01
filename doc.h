#ifndef __DOCSAMPLEH__
#define __DOCSAMPLEH__

#include "wx/docview.h"
#include "wx/cmdproc.h"

// Plots a line from one point to the other
class DoodleLine: public wxObject {
public:
    wxInt32 x1;
    wxInt32 y1;
    wxInt32 x2;
    wxInt32 y2;
};

// Contains a list of lines: represents a mouse-down doodle
class DoodleSegment: public wxObject {
public:
    wxList lines;

    DoodleSegment(){};
    DoodleSegment(const DoodleSegment& seg);
    ~DoodleSegment();

    void Draw(wxDC *dc);
};

class DrawingDocument: public wxDocument {
    DECLARE_DYNAMIC_CLASS(DrawingDocument)
private:
public:
    wxList doodleSegments;

    DrawingDocument(){};
    ~DrawingDocument();

    inline wxList& GetDoodleSegments() const { return (wxList&)doodleSegments; };
};

#define DOODLE_CUT          1
#define DOODLE_ADD          2

class DrawingCommand: public wxCommand {
protected:
    DoodleSegment *segment;
    DrawingDocument *doc;
    int cmd;
public:
    DrawingCommand(const wxString& name, int cmd, DrawingDocument *ddoc, DoodleSegment *seg);
    ~DrawingCommand();

    bool Do();
    bool Undo();
};


#endif
