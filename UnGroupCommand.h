#ifndef UNGROUPCOMMAND_H
#define UNGROUPCOMMAND_H

#include <Command.h>
#include <MyStorage.h>
#include <vector>

class CUnGroupCommand : public CCommand {
private:
    MyStorage* _storage = nullptr;
    Group* _group = nullptr;
    std::vector<CFigure*> members;

    int groupIndex = -1;
    bool captured = false;

public:
    CUnGroupCommand(MyStorage* s);

    void execute() override;
    void unexecute() override;
    std::string name() const override { return "UnGroup"; }
    CCommand* clone() override;
};

#endif // UNGROUPCOMMAND_H
