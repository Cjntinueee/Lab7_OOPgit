#ifndef FIGURES_H
#define FIGURES_H

#include <QPainter>
#include <QPalette>
#include <Factory.h>
#include <Object.h>
#include <Properties.h>


class CFigure: public CObject, public CProprties
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

    int _id;
    static int _nextId;
    QPen pen;

    struct PropertyDesc {
        QString name;
        enum Type { Int, Color } type;
        bool editable;
        std::function<QVariant()> getter;
        std::function<void(const QVariant&)> setter;
    };
    //virtual std::vector<PropertyDesc> GetProperties();

public:

    CFigure();
    CFigure(int x, int y);
    virtual ~CFigure() {}

    virtual int Width() const;
    virtual int Height() const;
    void ScreenIns(int winW, int winH);

    virtual bool MouseIn(int x, int y);
    virtual void paintt(QPainter *p);
    virtual void SizeChange(int size, int winW, int winH);

    bool Selected() const;
    void SetSel(bool sel);
    virtual void Move(int dx, int dy, int winW, int winH, bool notifyObs = true, int token = 0);
    virtual void ClearSel();

    virtual void SetPen(QColor c1);
    QColor GetColorP() const;

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

    int Id() const { return _id; }
    virtual CFigure* clone() const = 0;

    virtual void SetPenWidth(int w);
    int GetPenWidth() const;

    enum class PropId { PenColor, PenWidth, Width, Height };

    //virtual bool HasProp(PropId id) const;
    //virtual QVariant GetProp(PropId id) const;
    //virtual bool SetProp(PropId id, const QVariant& v);

    static constexpr int BasePropCount = 3; /////////

    int propCount() const override;
    PropMeta propMeta(int index) const override;
    QVariant getProp(int index) const override;
    bool setProp(int index, const QVariant& v) override;

};


#endif // FIGURES_H
