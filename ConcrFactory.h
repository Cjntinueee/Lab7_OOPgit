#ifndef CONCRFACTORY_H
#define CONCRFACTORY_H

#include <Factory.h>

class ConcrFactory: public CFactory{
public:

    CFigure* create(const std::string& type) override;
};
#endif // CONCRFACTORY_H
