#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <Figures.h>

class CEllipse: public CFigure
{

private:
    int rad1;
    int rad2;

public:

    CEllipse(int x, int y, int rad1, int rad2);
    ~CEllipse() {}

    void paintt(QPainter *p) override;
    bool MouseIn(int x, int y) override;
    void SizeChange(int rad2, int winW, int winH) override;

    int Width() const override;
    int Height() const override;

    QString Type() const override { return "Ellipse"; }
    void save(std::ostream& out) const override;
    void load(std::istream& in, CFactory* factory) override;

    CFigure* clone() const override;

    int GetRad1() const { return rad1; }
    int GetRad2() const { return rad2; }
    void SetRad1(int r) { rad1 = r; }
    void SetRad2(int r) { rad2 = r; }

    int propCount() const override;
    PropMeta propMeta(int index) const override;
    QVariant getProp(int index) const override;
    bool setProp(int index, const QVariant& v) override;
};



#endif // ELLIPSE_H
