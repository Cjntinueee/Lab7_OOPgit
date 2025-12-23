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

    int Width() override;
    int Height() override;

    QString Type() const override { return "Ellipse"; }
    void save(std::ostream& out) const override;
    void load(std::istream& in, CFactory* factory) override;
};



#endif // ELLIPSE_H
