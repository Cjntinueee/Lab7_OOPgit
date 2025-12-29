#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Figures.h>
#include <ostream>
#include <istream>

class CTriangle : public CFigure
{
private:

    QPoint a;
    QPoint b;
    QPoint c;
    void setSize(int newW, int newH);

public:
    CTriangle(QPoint p1, QPoint p2);
    ~CTriangle(){}

    void paintt(QPainter *p) override;
    bool MouseIn(int x, int y) override;
    void SizeChange(int size, int winW, int winH) override;

    void Move(int dx, int dy, int winW, int winH, bool notifyObs = true, int token = 0) override;
    int Width() const override;
    int Height() const override;

    QString Type() const override { return "Triangle"; }
    void save(std::ostream& out) const override;
    void load(std::istream& in, CFactory* factory) override;

    CFigure* clone() const override;

    int propCount() const override;
    PropMeta propMeta(int index) const override;
    QVariant getProp(int index) const override;
    bool setProp(int index, const QVariant& v) override;
    QPolygonF polygon() const { return QPolygonF() << a << b << c; }
};



#endif // TRIANGLE_H
