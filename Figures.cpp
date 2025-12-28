#include <Figures.h>
#include <QEvent>
#include <QPainter>
#include <QMouseEvent>



//
//Figure
//
int CFigure::_nextId = 1;
/*
CFigure::CFigure()
{
    x = 0;
    y = 0;
    select = false;
    colorP = Qt::white;
}
*/
CFigure::CFigure(int x, int y)
{
    this->x = x;
    this->y = y;
    select = false;
    colorP = Qt::white;
    pen = QPen(colorP);
    pen.setWidth(1);

}



bool CFigure::MouseIn(int x, int y)
{
    return false;
}

void CFigure::paintt(QPainter *p)
{
}

void CFigure::SizeChange(int size, int winW, int winH){

}



bool CFigure::Selected() const
{
    return select;
}

void CFigure::SetSel(bool sel)
{
    select = sel;
}


int CFigure::Width() const { return 0; }

int CFigure::Height() const { return 0; }

void CFigure::Move(int dx, int dy, int winW, int winH, bool notifyObs, int token) {
    x += dx;
    y += dy;
    ScreenIns(winW, winH);
    lastDx = dx;
    lastDy = dy;
    lastMoveToken = token;

    if (notifyObs && (dx != 0 || dy != 0)) {
        notify();
    }
}

void CFigure::ClearSel(){
    SetSel(false);
}

void CFigure::ScreenIns(int winW, int winH)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    int w = Width();
    int h = Height();

    if (x + w > winW) x = winW - w;
    if (y + h > winH) y = winH - h;
}


void CFigure::SetPen(QColor cp){
    colorP = cp;
    pen.setColor(cp);
    if (pen.width() <= 0) pen.setWidth(1);
}

QColor CFigure::GetColorP() const {
    return colorP;
}

void CFigure::SetInGroup(bool ig){
    InGroup = ig;
}

void CFigure::SetPenWidth(int w)
{
    pen.setWidth(w);
}

int CFigure::GetPenWidth() const
{
    return pen.width();
}


int CFigure::propCount() const {
    return BasePropCount;
}

PropMeta CFigure::propMeta(int i) const {
    PropMeta m;
    switch (i) {
    case 0: m.name = "Selected";  m.type = PropMeta::Type::Bool;  m.editable = true; break;
    case 1: m.name = "PenColor";  m.type = PropMeta::Type::Color; m.editable = true; break;
    case 2: m.name = "PenWidth";  m.type = PropMeta::Type::Int;   m.editable = true; m.minInt = 1; m.maxInt = 20; break;
    default: m.name = ""; m.editable = false; break;
    }
    return m;
}

QVariant CFigure::getProp(int i) const {
    switch (i) {
    case 0: return QVariant(Selected());
    case 1: return QVariant(GetColorP());
    case 2: return QVariant(GetPenWidth());
    default: return {};
    }
}

bool CFigure::setProp(int i, const QVariant& v) {
    switch (i) {
    case 0:
        SetSel(v.toBool());
        return true;
    case 1: {
        if (!v.canConvert<QColor>()) return false;
        SetPen(v.value<QColor>());
        return true;
    }
    case 2: {
        int w = v.toInt();
        if (w < 1) w = 1;
        if (w > 20) w = 20;
        SetPenWidth(w);
        return true;
    }
    default:
        return false;
    }
}






