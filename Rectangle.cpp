#include <Rectangle.h>

//
//Rectangle
//


CRect::CRect(int x, int y, int width, int height){

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    select = false;
}

void CRect::paintt(QPainter *p){

    if (this->Selected()){
        p->setPen(QPen(colorP));
        p->setBrush(QBrush(QColor(0, 120, 215, 60)));
    } else {
        p->setPen(QPen(colorP));
        p->setBrush(QBrush(Qt::transparent));
    }

    p->drawRect(x, y, width, height);
}
bool CRect::MouseIn(int mx, int my){
    return (mx >= this->x && mx <= this->x + width &&
            my >= this->y && my <= this->y + height);
}


void CRect::SizeChange(int w, int winW, int winH){
    double d = width / double(w);
    width = w;
    height /= d;
    ScreenIns(winW, winH);
}

int CRect::Width() const {
    return width;
}
int CRect::Height() const {
    return height;
}


void CRect::save(std::ostream& out) const {
    out << Type().toStdString() << '\n'
        << x << ' ' << y << ' '
        << width << ' ' << height << '\n'
        << colorP.red() << ' '
        << colorP.green() << ' '
        << colorP.blue()
        << '\n';
}
void CRect::load(std::istream& in, CFactory* factory) {

    int r, g, b;
    in >> x >> y >> width >> height;
    in >> r >> g >> b;
    colorP = QColor(r, g, b);
}


CFigure* CRect::clone() const {
    auto* r = new CRect(*this);
    r->SetSel(false);
    r->SetInGroup(false);
    return r;
}

int CRect::propCount() const {
    return CFigure::BasePropCount + 2;
}

PropMeta CRect::propMeta(int idx) const {
    if (idx < CFigure::BasePropCount) return CFigure::propMeta(idx);

    PropMeta m;
    int j = idx - CFigure::BasePropCount;
    if (j == 0) { m.name="RectW"; m.type=PropMeta::Type::Int; m.editable=true; m.minInt=1; m.maxInt=10000; }
    if (j == 1) { m.name="RectH"; m.type=PropMeta::Type::Int; m.editable=true; m.minInt=1; m.maxInt=10000; }
    return m;
}

QVariant CRect::getProp(int idx) const {
    if (idx < CFigure::BasePropCount) return CFigure::getProp(idx);

    int j = idx - CFigure::BasePropCount;
    if (j == 0) return QVariant(GetW());
    if (j == 1) return QVariant(GetH());
    return {};
}

bool CRect::setProp(int idx, const QVariant& v) {
    if (idx < CFigure::BasePropCount) return CFigure::setProp(idx, v);

    int j = idx - CFigure::BasePropCount;
    int val = v.toInt();
    if (val < 1) val = 1;

    if (j == 0) { SetW(val); return true; }
    if (j == 1) { SetH(val); return true; }
    return false;
}
