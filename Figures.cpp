#include <Figures.h>
#include <QEvent>
#include <QPainter>
#include <QMouseEvent>



//
//Figure
//


CFigure::CFigure()
{
    x = 0;
    y = 0;
    select = false;
    colorP = Qt::white;
}

CFigure::CFigure(int x, int y)
{
    this->x = x;
    this->y = y;
    select = false;
    colorP = Qt::white;

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



bool CFigure::Selected()
{
    return select;
}

void CFigure::SetSel(bool sel)
{
    select = sel;
}


int CFigure::Width() { return 0; }

int CFigure::Height(){ return 0; }

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
}

QColor CFigure::GetColorP(){
    return colorP;
}

void CFigure::SetInGroup(bool ig){
    InGroup = ig;
}











