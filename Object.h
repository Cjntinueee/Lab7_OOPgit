#ifndef OBJECT_H
#define OBJECT_H
#include <Observer.h>
#include <vector>
#include <algorithm>

class CObject {
private:
    std::vector<CObserver*> observers;
public:
    void addObs(CObserver* obs);
    void removeObs(CObserver* obs);
    void notify();
    virtual ~CObject(){}
};


#endif // OBJECT_H
