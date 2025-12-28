#ifndef ARROW_H
#define ARROW_H
#include <Figures.h>
#include <Observer.h>
#include <QPainter>
#include <cmath>
#include <ostream>

class CArrow: public CFigure, public CObserver {
private:
    CFigure* A = nullptr;
    CFigure* B = nullptr;

    int winW = 0;
    int winH = 0;

    bool bidirectional = false;

    bool propagating = false;
    //bool SelectA = false;
public:
    CArrow(CFigure* a, CFigure* b, bool bid = false) : A(a), B(b), bidirectional(bid) {
        if (A) A->addObs(this);
        if (bidirectional && B) B->addObs(this);
    }

    ~CArrow() override {
        if (A) A->removeObs(this);
        if (bidirectional && B) B->removeObs(this);
    }

    bool MouseIn(int mx, int my) override;

    void SetViewport(int w, int h) { winW = w; winH = h; }

    QString Type() const override { return "AR"; }

    void Move(int, int, int, int, bool = true, int = 0) override {}

    void paintt(QPainter* p) override;
    void drawArr(QPainter* p, const QPoint& p1, const QPoint& p2, bool selected);

    void OnSubjChanged(CObject* who) override;

    CFigure* Src() const { return A; }
    CFigure* Dst() const { return B; }

    void save(std::ostream& out) const override {}

    void SetBidirectional(bool bid);
    bool IsBidirectional() const { return bidirectional; }
};
#endif // ARROW_H
