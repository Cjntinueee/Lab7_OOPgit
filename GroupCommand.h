#ifndef GROUPCOMMAND_H
#define GROUPCOMMAND_H

#include <Command.h>
#include <MyStorage.h>

class CGroupCommand: public CCommand {
private:
    MyStorage* _storage;
    Group* created = nullptr;
public:
    CGroupCommand(MyStorage* storage);
    void execute() override;
    void unexecute() override;
    std::string name() const override { return "Group"; }
    CCommand* clone() override;
};
#endif // GROUPCOMMAND_H
