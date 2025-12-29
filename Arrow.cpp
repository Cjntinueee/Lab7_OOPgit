#include "Arrow.h"

QPoint centerOf(CFigure* f) {
    if (!f) return QPoint(0,0);
    return QPoint(f->GetX() + f->Width()/2, f->GetY() + f->Height()/2);
}

double distancePointToSegment(
    double px, double py,
    double ax, double ay,
    double bx, double by)
{
    double vx = bx - ax;
    double vy = by - ay;

    double wx = px - ax;
    double wy = py - ay;

    double c1 = vx * wx + vy * wy;
    if (c1 <= 0)
        return std::sqrt((px - ax)*(px - ax) + (py - ay)*(py - ay));

    double c2 = vx * vx + vy * vy;
    if (c2 <= c1)
        return std::sqrt((px - bx)*(px - bx) + (py - by)*(py - by));

    double t = c1 / c2;

    double projX = ax + t * vx;
    double projY = ay + t * vy;

    return std::sqrt((px - projX)*(px - projX) +
                     (py - projY)*(py - projY));
}

void CArrow::drawArr(QPainter* p, const QPoint& p1, const QPoint& p2, bool selected)
{
    const double angle = std::atan2(double(p2.y() - p1.y()), double(p2.x() - p1.x()));
    const double len = 12.0;
    const double a1 = angle + M_PI * 0.85;
    const double a2 = angle - M_PI * 0.85;

    QPointF h1(p2.x() + len * std::cos(a1), p2.y() + len * std::sin(a1));
    QPointF h2(p2.x() + len * std::cos(a2), p2.y() + len * std::sin(a2));

    QPolygonF head;
    head << p2 << h1 << h2;
    if (selected){
        p->setPen(QPen(QColor(0, 120, 215, 255), 2));
        p->setBrush(QColor(0, 120, 215, 255));
    } else{
        p->setPen(QPen(Qt::white, 2));
        p->setBrush(Qt::white);
    }
    p->drawLine(p1, p2);
    p->drawPolygon(head);
}


void CArrow::OnSubjChanged(CObject* who)
{
    //if (isPropagating) return;
/*
    int token = A->GetLastMoveToken();
    if (token != 0 && B->GetLastMoveToken() == token) {
        return;
    }

    int dx = A->GetLastDx();
    int dy = A->GetLastDy();
    if (dx == 0 && dy == 0) return;

    //isPropagating = true;
    B->Move(dx, dy, winW, winH, true, token);
    //isPropagating = false;*/

    if (!A || !B) return;

    if (propagating) return;

    CFigure* src = nullptr;
    CFigure* dst = nullptr;

    if (who == A) { src = A; dst = B; }
    else if (bidirectional && who == B) { src = B; dst = A; }
    else return;

    int dx = src->GetLastDx();
    int dy = src->GetLastDy();
    if (dx == 0 && dy == 0) return;

    int token = src->GetLastMoveToken();

    if (token != 0 && dst->GetLastMoveToken() == token) return;

    int w = (winW > 0 ? winW : 1000000);
    int h = (winH > 0 ? winH : 1000000);

    propagating = true;
    dst->Move(dx, dy, w, h, true, token);
    propagating = false;
}

void CArrow::paintt(QPainter* p)
{
    if (!A || !B) return;

    QPoint pA = centerOf(A);
    QPoint pB = centerOf(B);

    drawArr(p, pA, pB, select);

    if (bidirectional) {
        drawArr(p, pB, pA, select);
    }
}


bool CArrow::MouseIn(int mx, int my){
    if (!Src() || !Dst())
        return false;

    int x1 = Src()->GetX() + Src()->Width() / 2;
    int y1 = Src()->GetY() + Src()->Height() / 2;
    int x2 = Dst()->GetX() + Dst()->Width() / 2;
    int y2 = Dst()->GetY() + Dst()->Height() / 2;

    const double tolerance = 6.0;
    double d = distancePointToSegment(
        mx, my,
        x1, y1,
        x2, y2
        );

    return d <= tolerance;
}

void CArrow::SetBidirectional(bool bid) {
    if (bidirectional == bid) return;
    if (bidirectional && B) B->removeObs(this);
    bidirectional = bid;
    if (bidirectional && B) B->addObs(this);
}


