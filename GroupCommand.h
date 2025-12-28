#ifndef GROUPCOMMAND_H
#define GROUPCOMMAND_H

#include <Command.h>
#include <MyStorage.h>

class CGroupCommand: public CCommand {
private:
    MyStorage* _storage;
    Group* created = nullptr;
    std::vector<CFigure*> grouped;

    //std::vector<int> groupedIds;
    //int createdId = -1;
    //int createdIndex = -1;
    std::vector<int> indices;
    int groupIndex = -1;
public:
    CGroupCommand(MyStorage* storage);
    void execute() override;
    void unexecute() override;
    std::string name() const override { return "Group"; }
    CCommand* clone() override;

    //Group* findCreatedGroup() const;
};
#endif // GROUPCOMMAND_H
