#ifndef FACTORY_H
#define FACTORY_H

#include <QString>

class CFigure;

class CFactory{
public:
    virtual ~CFactory() {}
    virtual CFigure* create(const std::string& type) = 0;
};



#endif // FACTORY_H
