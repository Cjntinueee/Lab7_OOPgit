#ifndef COMMAND_H
#define COMMAND_H

#include <Figures.h>

class CCommand {
public:
    virtual void execute() = 0;
    virtual void unexecute() = 0;
    virtual CCommand* clone() = 0;
    virtual std::string name() const { return "Command"; }
    virtual ~CCommand(){}
};

#endif // COMMAND_H
