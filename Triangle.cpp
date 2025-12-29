#include <Triangle.h>
#include <QPolygonF>



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
        p->setPen(pen);
        p->setBrush(QBrush(QColor(0, 120, 215, 60)));
    } else {
        p->setPen(pen);
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

    if (newA.x() < 0 || newA.y() < 0 || newA.x() > winW || newA.y() > winH) return;
    if (newB.x() < 0 || newB.y() < 0 || newB.x() > winW || newB.y() > winH) return;
    if (newC.x() < 0 || newC.y() < 0 || newC.x() > winW || newC.y() > winH) return;

    a = newA;
    b = newB;
    c = newC;

    x += dx;
    y += dy;

    lastDx = dx;
    lastDy = dy;
    lastMoveToken = token;

    if (notifyObs && (dx != 0 || dy != 0)) {
        notify();
    }
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
        << pen.color().red() << ' '
        << pen.color().green() << ' '
        << pen.color().blue() << ' '
        << pen.width() << ' '
        << "\n";
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

    SetPen(QColor(r, g, bcol));
    select = false;
    int pw;
    in >> pw;
    pen.setWidth(pw);
}




CFigure* CTriangle::clone() const {
    auto* e = new CTriangle(*this);
    e->SetSel(false);
    e->SetInGroup(false);
    return e;
}

int CTriangle::propCount() const {
    return CFigure::BasePropCount + 2;
}

PropMeta CTriangle::propMeta(int idx) const {
    if (idx < CFigure::BasePropCount) return CFigure::propMeta(idx);

    PropMeta m;
    int j = idx - CFigure::BasePropCount;

    if (j == 0) { m.name = "TriW"; m.type = PropMeta::Type::Int; m.editable = true; m.minInt = 1; m.maxInt = 10000; }
    if (j == 1) { m.name = "TriH"; m.type = PropMeta::Type::Int; m.editable = true; m.minInt = 1; m.maxInt = 10000; }
    return m;
}

QVariant CTriangle::getProp(int idx) const {
    if (idx < CFigure::BasePropCount) return CFigure::getProp(idx);

    int j = idx - CFigure::BasePropCount;
    if (j == 0) return QVariant(Width());
    if (j == 1) return QVariant(Height());
    return {};
}

bool CTriangle::setProp(int idx, const QVariant& v) {
    if (idx < CFigure::BasePropCount) return CFigure::setProp(idx, v);

    int j = idx - CFigure::BasePropCount;
    int val = v.toInt();
    if (val < 1) val = 1;

    if (j == 0) { setSize(val, Height()); return true; }
    if (j == 1) { setSize(Width(), val); return true; }
    return false;
}


void CTriangle::setSize(int newW, int newH) {
    if (newW < 1) newW = 1;
    if (newH < 1) newH = 1;

    int minX = std::min({a.x(), b.x(), c.x()});
    int minY = std::min({a.y(), b.y(), c.y()});

    int oldW = Width();
    int oldH = Height();
    if (oldW < 1) oldW = 1;
    if (oldH < 1) oldH = 1;

    const double sx = double(newW) / double(oldW);
    const double sy = double(newH) / double(oldH);

    auto scalePt = [&](const QPoint& p) {
        int nx = int(std::lround(minX + (p.x() - minX) * sx));
        int ny = int(std::lround(minY + (p.y() - minY) * sy));
        return QPoint(nx, ny);
    };

    a = scalePt(a);
    b = scalePt(b);
    c = scalePt(c);

    x = std::min({a.x(), b.x(), c.x()});
    y = std::min({a.y(), b.y(), c.y()});

    notify();
}









