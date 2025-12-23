#ifndef GROUP_H
#define GROUP_H

#include <Figures.h>
#include <QPainter>
#include <ostream>
#include <istream>

class Group: public CFigure {
private:
    CFigure** group;
    int countG;

public:
    Group(int size);
    ~Group();

    void addtoGr(CFigure* f);
    void paintt(QPainter *p) override;
    void SetPen(QColor c) override;

    int Width() override;
    int Height() override;

    bool MouseIn(int x, int y) override;
    void Move(int dx, int dy, int winW, int winH, bool notifyObs = true, int token = 0) override;

    void ClearSel() override;

    int GetCountG();
    CFigure* GetFigure(int i);

    QString Type() const override { return "G"; }
    void save(std::ostream& out) const override;
    void load(std::istream& in, CFactory* factory) override;

    CFigure* FigureAt(int i) const;
};

#endif // GROUP_H
