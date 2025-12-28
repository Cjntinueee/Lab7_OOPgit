#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <Figures.h>
#include <ostream>
#include <istream>

class CRect: public CFigure
{

private:
    int width;
    int height;

public:

    CRect(int x, int y, int width, int height);
    ~CRect(){}

    void paintt(QPainter *p) override;
    bool MouseIn(int mx, int my) override;
    void SizeChange(int w, int winW, int winH) override;

    int Width() const override;
    int Height() const override;

    QString Type() const override { return "Rectangle"; }
    void save(std::ostream& out) const override;
    void load(std::istream& in, CFactory* factory) override;

    CFigure* clone() const override;

    int GetW() const { return width; }
    int GetH() const { return height; }
    void SetW(int w) { width = w; }
    void SetH(int h) { height = h; }

    int propCount() const override;
    PropMeta propMeta(int index) const override;
    QVariant getProp(int index) const override;
    bool setProp(int index, const QVariant& v) override;
};


#endif // RECTANGLE_H
