#ifndef OBSERVER_H
#define OBSERVER_H

class CObject;

class CObserver {
public:
    virtual void OnSubjChanged(CObject* who) = 0;
    virtual ~CObserver() {}
};

#endif // OBSERVER_H
