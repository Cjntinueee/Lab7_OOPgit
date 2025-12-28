#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <Command.h>
#include <MyStorage.h>

class CMoveCommand: public CCommand {
private:
    MyStorage* _storage;
    int _dx, _dy;
    std::vector<CFigure*> targets;

    int winW, winH;
    int token;
    //bool firstExecute = true;
public:
    CMoveCommand(MyStorage* s, int dx, int dy, int w, int h, int token);
    void execute() override;
    void unexecute() override;
    std::string name() const override { return "Move"; }
    CCommand* clone() override;
};

#endif // MOVECOMMAND_H
