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
        p->setPen(QPen(colorP));
        p->setBrush(QBrush(QColor(0, 120, 215, 60)));
    } else {
        p->setPen(QPen(colorP));
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

int CEllipse::Width(){
    return 2*rad1;
}

int CEllipse::Height(){
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














