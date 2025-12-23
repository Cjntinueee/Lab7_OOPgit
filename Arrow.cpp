#include "Arrow.h"
/*
static QPoint centerOf(CFigure* f) {
    if (!f) return QPoint(0,0);
    return QPoint(f->GetX() + f->Width()/2, f->GetY() + f->Height()/2);
}

void CArrow::OnSubjChanged(CObject* who)
{
    if (isPropagating) return;

    int token = A->GetLastMoveToken();
    if (token != 0 && B->GetLastMoveToken() == token) {
        return;
    }

    int dx = A->GetLastDx();
    int dy = A->GetLastDy();
    if (dx == 0 && dy == 0) return;

    isPropagating = true;
    B->Move(dx, dy, winW, winH, true, token);
    isPropagating = false;

}

void CArrow::paintt(QPainter* p)
{
    if (!A || !B) return;

    QPoint p1 = centerOf(A);
    QPoint p2 = centerOf(B);

    p->setPen(QPen(Qt::white, 2));
    p->drawLine(p1, p2);

    const double angle = std::atan2(double(p2.y() - p1.y()), double(p2.x() - p1.x()));
    const double len = 12.0;
    const double a1 = angle + M_PI * 0.85;
    const double a2 = angle - M_PI * 0.85;

    QPointF h1(p2.x() + len * std::cos(a1), p2.y() + len * std::sin(a1));
    QPointF h2(p2.x() + len * std::cos(a2), p2.y() + len * std::sin(a2));

    QPolygonF head;
    head << p2 << h1 << h2;

    p->setBrush(Qt::white);
    p->drawPolygon(head);
}
*/

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


void CArrow::OnSubjChanged(CObject* who)
{
    if (isPropagating) return;

    int token = A->GetLastMoveToken();
    if (token != 0 && B->GetLastMoveToken() == token) {
        return;
    }

    int dx = A->GetLastDx();
    int dy = A->GetLastDy();
    if (dx == 0 && dy == 0) return;

    isPropagating = true;
    B->Move(dx, dy, winW, winH, true, token);
    isPropagating = false;

}

void CArrow::paintA(QPainter* p)
{
    if (!A || !B) return;

    QPoint p1 = centerOf(A);
    QPoint p2 = centerOf(B);

    const double angle = std::atan2(double(p2.y() - p1.y()), double(p2.x() - p1.x()));
    const double len = 12.0;
    const double a1 = angle + M_PI * 0.85;
    const double a2 = angle - M_PI * 0.85;

    QPointF h1(p2.x() + len * std::cos(a1), p2.y() + len * std::sin(a1));
    QPointF h2(p2.x() + len * std::cos(a2), p2.y() + len * std::sin(a2));

    QPolygonF head;
    head << p2 << h1 << h2;
    if (SelectA){
        p->setPen(QPen(QColor(0, 120, 215, 255), 2));
        p->setBrush(QColor(0, 120, 215, 255));
    } else{
        p->setPen(QPen(Qt::white, 2));
        p->setBrush(Qt::white);
    }
    p->drawLine(p1, p2);
    p->drawPolygon(head);
}


bool CArrow::MouseInA(int mx, int my){
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


