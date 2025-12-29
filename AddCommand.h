#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H


#include <Command.h>
#include <MyStorage.h>

class CAddCommand: public CCommand {
private:
    CFigure* _f;
    MyStorage* _storage;

    struct ArrowInfo {
        CFigure* a = nullptr;
        CFigure* b = nullptr;
        bool bid = false;
    };
    std::vector<ArrowInfo> removedArrows;

    bool captured = false;
public:
    CAddCommand(CFigure* f, MyStorage* storage);
    void execute() override;
    void unexecute() override;
    std::string name() const override { return "Add"; }
    CCommand* clone() override;
};
#endif // ADDCOMMAND_H
