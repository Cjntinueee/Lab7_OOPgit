#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H


#include <Command.h>
#include <MyStorage.h>

class CAddCommand: public CCommand {
private:
    CFigure* _f;
    MyStorage* _storage;
public:
    CAddCommand(CFigure* f, MyStorage* storage);
    void execute() override;
    void unexecute() override;
    std::string name() const override { return "Add"; }
    CCommand* clone() override;
};
#endif // ADDCOMMAND_H
