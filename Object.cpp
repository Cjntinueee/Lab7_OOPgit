#include <Object.h>


void CObject::addObs(CObserver* obs){
    if (!obs) return;
    if (std::find(observers.begin(), observers.end(), obs) == observers.end())
        observers.push_back(obs);
}


void CObject::notify(){
    for (int i = 0; i < observers.size(); i++){
        observers[i]->OnSubjChanged(this);
    }
}


void CObject::removeObs(CObserver* obs) {
    observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
}
