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

    int Width() const override;
    int Height() const override;

    bool MouseIn(int x, int y) override;
    void Move(int dx, int dy, int winW, int winH, bool notifyObs = true, int token = 0) override;

    void ClearSel() override;

    int GetCountG();
    CFigure* GetFigure(int i);

    QString Type() const override { return "Group"; }
    void save(std::ostream& out) const override;
    void load(std::istream& in, CFactory* factory) override;

    CFigure* FigureAt(int i) const;

    CFigure* clone() const override;
    void ClearCount() { countG = 0;}

    void SetPenWidth(int w) override;
};

#endif // GROUP_H
