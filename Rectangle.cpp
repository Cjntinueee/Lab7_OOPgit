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

int CRect::Width(){
    return width;
}
int CRect::Height(){
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




