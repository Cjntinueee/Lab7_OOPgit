#ifndef ARROW_H
#define ARROW_H
#include <Figures.h>
#include <Observer.h>
#include <QPainter>
#include <cmath>
#include <ostream>
/*
class CArrow: public CFigure, public CObserver {
private:
    CFigure* A = nullptr;
    CFigure* B = nullptr;

    int winW = 0;
    int winH = 0;

    bool isPropagating = false;
public:
    CArrow(CFigure* a, CFigure* b) : A(a), B(b) {
        if (A) A->addObs(this);
    }

    ~CArrow() override {
        if (A) A->removeObs(this);
    }

    void SetViewport(int w, int h) { winW = w; winH = h; }

    QString Type() const override { return "AR"; }

    void Move(int, int, int, int, bool = true, int = 0) override {}

    void paintt(QPainter* p) override;

    void OnSubjChanged(CObject* who) override;

    CFigure* Src() const { return A; }
    CFigure* Dst() const { return B; }

    void save(std::ostream& out) const override {
        out << Type().toStdString() << "\n";
    }

};
*/

class CArrow: public CObserver {
private:
    CFigure* A = nullptr;
    CFigure* B = nullptr;

    int winW = 0;
    int winH = 0;

    bool isPropagating = false;
    bool SelectA = false;
public:
    CArrow(CFigure* a, CFigure* b) : A(a), B(b) {
        if (A) A->addObs(this);
    }

    ~CArrow() override {
        if (A) A->removeObs(this);
    }

    bool MouseInA(int mx, int my); //
    void SetSelA(bool sel){SelectA = sel;}
    bool GetSelA(){return SelectA;}

    void SetViewport(int w, int h) { winW = w; winH = h; }

    QString TypeA() const { return "AR"; }

    //void MoveA(int, int, int, int, bool = true, int = 0){}

    void paintA(QPainter* p);

    void OnSubjChanged(CObject* who) override;

    CFigure* Src() const { return A; }
    CFigure* Dst() const { return B; }

    //void saveA(std::ostream& out) const;
};
#endif // ARROW_H
