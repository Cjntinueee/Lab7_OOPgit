#include <ostream>
#include <istream>
#include <Ellipse.h>


//
//Ellipse
//



CEllipse::CEllipse(int x, int y, int rad1, int rad2){
    this->x = x;
    this->y = y;
    this->rad1 = rad1;
    this->rad2 = rad2;
    select = false;
}


void CEllipse::paintt(QPainter *p){

    if (this->Selected()){
        p->setPen(pen);
        p->setBrush(QBrush(QColor(0, 120, 215, 60)));
    } else {
        p->setPen(pen);
        p->setBrush(QBrush(Qt::transparent));
    }

    p->drawEllipse(x, y, 2*rad1, 2*rad2);
}

bool CEllipse::MouseIn(int mx, int my){
    int cx = x + rad1;
    int cy = y + rad2;

    double dx = (mx - cx) / double(rad1);
    double dy = (my - cy) / double(rad2);

    return dx*dx + dy*dy <= 1;
}

void CEllipse::SizeChange(int rad, int winW, int winH){
    double d = rad1 / double(rad);
    rad1 = rad;
    rad2 /= d;
    ScreenIns(winW, winH);
}

int CEllipse::Width() const{
    return 2*rad1;
}

int CEllipse::Height() const{
    return 2*rad2;
}


void CEllipse::save(std::ostream& out) const {
    out << Type().toStdString() << '\n'
        << x << ' ' << y << ' '
        << rad1 << ' ' << rad2 << '\n'
        << colorP.red() << ' '
        << colorP.green() << ' '
        << colorP.blue()
        << '\n';
}
void CEllipse::load(std::istream& in, CFactory* factory) {

    int r, g, b;
    in >> x >> y >> rad1 >> rad2;
    in >> r >> g >> b;
    colorP = QColor(r, g, b);
}


CFigure* CEllipse::clone() const {
    auto* e = new CEllipse(*this);
    e->SetSel(false);
    e->SetInGroup(false);
    return e;
}

int CEllipse::propCount() const {
    return CFigure::BasePropCount + 2;
}

PropMeta CEllipse::propMeta(int idx) const {
    if (idx < CFigure::BasePropCount) return CFigure::propMeta(idx);

    PropMeta m;
    int j = idx - CFigure::BasePropCount;
    if (j == 0) { m.name="RadX"; m.type=PropMeta::Type::Int; m.editable=true; m.minInt=1; m.maxInt=10000; }
    if (j == 1) { m.name="RadY"; m.type=PropMeta::Type::Int; m.editable=true; m.minInt=1; m.maxInt=10000; }
    return m;
}

QVariant CEllipse::getProp(int idx) const {
    if (idx < CFigure::BasePropCount) return CFigure::getProp(idx);

    int j = idx - CFigure::BasePropCount;
    if (j == 0) return QVariant(GetRad1());
    if (j == 1) return QVariant(GetRad2());
    return {};
}

bool CEllipse::setProp(int idx, const QVariant& v) {
    if (idx < CFigure::BasePropCount) return CFigure::setProp(idx, v);

    int j = idx - CFigure::BasePropCount;
    int val = v.toInt();
    if (val < 1) val = 1;

    if (j == 0) { SetRad1(val); return true; }
    if (j == 1) { SetRad2(val); return true; }
    return false;
}









