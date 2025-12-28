#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QString>
#include <QVariant>

struct PropMeta {
    enum class Type { Int, Bool, Color };

    QString name;
    Type type = Type::Int;
    bool editable = true;

    int minInt = -100000;
    int maxInt =  100000;
};

class CProprties {
public:
    virtual ~CProprties() = default;

    virtual int propCount() const = 0;
    virtual PropMeta propMeta(int index) const = 0;

    virtual QVariant getProp(int index) const = 0;
    virtual bool setProp(int index, const QVariant& v) = 0;
};

#endif // PROPERTIES_H
