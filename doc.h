#ifndef DOC_H
#define DOC_H

#include "wx/docview.h"
#include "wx/cmdproc.h"

// 線のオブジェクト
class DrawLine: public wxObject {
public:
    wxInt32 x1;
    wxInt32 y1;
    wxInt32 x2;
    wxInt32 y2;
};

// セグメント（繋がった線のリスト）
// 最大、最小のx,yを追加する
class DrawSegment : public wxObject {
public:
    wxList lines;
    
    // セグメントを囲う矩形
    wxRect rect;

    DrawSegment(){};
    DrawSegment(const DrawSegment& seg);
    ~DrawSegment();

    void Draw(wxDC *dc);
};

// ドキュメント（線のリストのリスト）
class DrawingDocument : public wxDocument {
    DECLARE_DYNAMIC_CLASS(DrawingDocument)
private:
public:
    wxList drawSegments;

    DrawingDocument(){};
    ~DrawingDocument();

    inline wxList& GetDrawSegments() const { return (wxList&)drawSegments; };
};

enum {
    DRAW_DELETE,
    DRAW_ADD
};

class DrawingCommand: public wxCommand {
protected:
    DrawSegment *segment;
    DrawingDocument *doc;
    int cmd;
public:
    DrawingCommand(const wxString& name, int cmd, DrawingDocument *ddoc, DrawSegment *seg);
    ~DrawingCommand();

    bool Do();
    bool Undo();
};


#endif
