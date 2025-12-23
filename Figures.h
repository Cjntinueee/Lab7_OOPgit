#ifndef FIGURES_H
#define FIGURES_H

#include <QPainter>
#include <QPalette>
#include <Factory.h>
#include <Object.h>


class CFigure: public CObject
{
protected:
    int x;
    int y;
    bool select;
    QColor colorP;
    int lastDx = 0;
    int lastDy = 0;

    bool moveNotifyEnabled = true;
    int lastMoveToken = 0;

    bool InGroup = false;

public:

    CFigure();
    CFigure(int x, int y);
    virtual ~CFigure() {}

    virtual int Width();
    virtual int Height();
    void ScreenIns(int winW, int winH);

    virtual bool MouseIn(int x, int y);
    virtual void paintt(QPainter *p);
    virtual void SizeChange(int size, int winW, int winH);

    bool Selected();
    void SetSel(bool sel);
    virtual void Move(int dx, int dy, int winW, int winH, bool notifyObs = true, int token = 0);
    virtual void ClearSel();

    virtual void SetPen(QColor c1);
    QColor GetColorP();

    int GetX(){ return x; }
    int GetY(){ return y; }

    int GetLastDx() const { return lastDx; }
    int GetLastDy() const { return lastDy; }
    int GetLastMoveToken() const { return lastMoveToken; }

    virtual QString Type() const = 0;

    virtual void save(std::ostream& out) const = 0;
    virtual void load(std::istream& in, CFactory* factory) {}

    void SetInGroup(bool ig);
    bool GetInGroup() {return InGroup;}

};


#endif // FIGURES_H
