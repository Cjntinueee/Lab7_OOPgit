#include <Triangle.h>




//
// Triangle
//


CTriangle::CTriangle(QPoint p1, QPoint p2)
{

    int x1 = std::min(p1.x(), p2.x());
    int y1 = std::min(p1.y(), p2.y());
    int x2 = std::max(p1.x(), p2.x());
    int y2 = std::max(p1.y(), p2.y());


    a = QPoint(x1, y2);
    b = QPoint(x2, y2);
    c = QPoint((x1 + x2) / 2, y1);


    x = x1;
    y = y1;

    select = false;
}


void CTriangle::paintt(QPainter *p)
{
    if (Selected()) {
        p->setPen(QPen(colorP));
        p->setBrush(QBrush(QColor(0, 120, 215, 60)));
    } else {
        p->setPen(QPen(colorP));
        p->setBrush(Qt::NoBrush);
    }

    QPolygon poly;
    poly << a << b << c;
    p->drawPolygon(poly);
}

bool CTriangle::MouseIn(int mx, int my)
{
    QPolygon poly;
    poly << a << b << c;
    return poly.containsPoint(QPoint(mx, my), Qt::OddEvenFill);
}

void CTriangle::SizeChange(int size, int winW, int winH)
{
    if (size <= 0)
        return;

    int minX = std::min(std::min(a.x(), b.x()), c.x());
    int maxX = std::max(std::max(a.x(), b.x()), c.x());
    int minY = std::min(std::min(a.y(), b.y()), c.y());
    int maxY = std::max(std::max(a.y(), b.y()), c.y());

    int curW = maxX - minX;
    int curH = maxY - minY;

    if (curW <= 0 || curH <= 0)
        return;

    double k = double(size) / double(curW);

    QPoint newA, newB, newC;

    newA.setX(minX + (a.x() - minX) * k);
    newA.setY(minY + (a.y() - minY) * k);

    newB.setX(minX + (b.x() - minX) * k);
    newB.setY(minY + (b.y() - minY) * k);

    newC.setX(minX + (c.x() - minX) * k);
    newC.setY(minY + (c.y() - minY) * k);

    int nMinX = std::min(std::min(newA.x(), newB.x()), newC.x());
    int nMaxX = std::max(std::max(newA.x(), newB.x()), newC.x());
    int nMinY = std::min(std::min(newA.y(), newB.y()), newC.y());
    int nMaxY = std::max(std::max(newA.y(), newB.y()), newC.y());

    if (nMinX < 0 || nMinY < 0 || nMaxX > winW || nMaxY > winH)
        return;

    a = newA;
    b = newB;
    c = newC;

    x = nMinX;
    y = nMinY;
}


void CTriangle::Move(int dx, int dy, int winW, int winH, bool notifyObs, int token)
{
    QPoint newA(a.x() + dx, a.y() + dy);
    QPoint newB(b.x() + dx, b.y() + dy);
    QPoint newC(c.x() + dx, c.y() + dy);

    if (newA.x() < 0 || newA.y() < 0 || newA.x() > winW || newA.y() > winH)
        return;

    if (newB.x() < 0 || newB.y() < 0 || newB.x() > winW || newB.y() > winH)
        return;

    if (newC.x() < 0 || newC.y() < 0 || newC.x() > winW || newC.y() > winH)
        return;

    a = newA;
    b = newB;
    c = newC;

    x += dx;
    y += dy;
}


int CTriangle::Width() const
{
    int minX = a.x();
    int maxX = a.x();

    if (b.x() < minX) minX = b.x();
    if (c.x() < minX) minX = c.x();

    if (b.x() > maxX) maxX = b.x();
    if (c.x() > maxX) maxX = c.x();

    return maxX - minX;
}

int CTriangle::Height() const
{
    int minY = a.y();
    int maxY = a.y();

    if (b.y() < minY) minY = b.y();
    if (c.y() < minY) minY = c.y();

    if (b.y() > maxY) maxY = b.y();
    if (c.y() > maxY) maxY = c.y();

    return maxY - minY;
}



void CTriangle::save(std::ostream& out) const {
    out << Type().toStdString() << "\n"
        << a.x() << " " << a.y() << " "
        << b.x() << " " << b.y() << " "
        << c.x() << " " << c.y() << "\n"
        << colorP.red() << " " << colorP.green() << " " << colorP.blue() << "\n";
}
void CTriangle::load(std::istream& in, CFactory* factory) {

    int ax, ay, bx, by, cx, cy;
    int r, g, bcol;

    in >> ax >> ay >> bx >> by >> cx >> cy;
    in >> r >> g >> bcol;

    a = QPoint(ax, ay);
    b = QPoint(bx, by);
    c = QPoint(cx, cy);

    x = std::min({a.x(), b.x(), c.x()});
    y = std::min({a.y(), b.y(), c.y()});

    colorP = QColor(r, g, bcol);
    select = false;
}




CFigure* CTriangle::clone() const {
    auto* e = new CTriangle(*this);
    e->SetSel(false);
    e->SetInGroup(false);
    return e;
}
