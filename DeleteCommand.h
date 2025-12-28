#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <Command.h>
#include <MyStorage.h>

class CDeleteCommand: public CCommand {
private:
    MyStorage* _storage;
    std::vector<CFigure*> removedFigures;

    struct RemovedArrowInfo {
        CFigure* a;
        CFigure* b;
        bool bid;
    };
    std::vector<RemovedArrowInfo> removedArrows;
public:
    CDeleteCommand(MyStorage* storage);
    void execute() override;
    void unexecute() override;
    std::string name() const override { return "Delete"; }
    CCommand* clone() override;
};
#endif // DELETECOMMAND_H
