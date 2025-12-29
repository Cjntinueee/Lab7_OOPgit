#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H
#include <vector>
#include <Figures.h>
#include <stack>
#include <Command.h>
#include <memory>

class CCommandManager {
private:
    std::stack<CCommand*> _undo;
    std::stack<CCommand*> _redo;

public:
    std::vector<std::string> history;

    CCommandManager(){}
    void execute(CCommand* cmd);
    void undo();
    void redo();

    ~CCommandManager();
};

#endif // COMMANDMANAGER_H
