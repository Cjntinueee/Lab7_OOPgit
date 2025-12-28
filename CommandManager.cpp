#include <CommandManager.h>
#include <iostream>

void CCommandManager::execute(CCommand* cmd){
    /*cmd->execute();
    _undo.push(cmd);
    while (!_redo.empty()){
        _redo.pop();
    }*/
    if (!cmd) return;
    cmd->execute();
    _undo.push(cmd);
    std::cout << "[DO]   " << cmd->name() << std::endl;
    //history.push_back(cmd->name());
    while (!_redo.empty()) { delete _redo.top(); _redo.pop(); }

}

void CCommandManager::undo(){
    if (_undo.empty()) return;
    auto cmd = _undo.top();
    _undo.pop();
    cmd->unexecute();
    //history.push_back("UNDO: " + cmd->name());
    std::cout << "[UNDO] " << cmd->name() << std::endl;
    _redo.push(cmd);

}
void CCommandManager::redo(){
    if (_redo.empty()) return;
    auto cmd = _redo.top();
    _redo.pop();
    cmd->execute();
    //history.push_back("REDO: " + cmd->name());
    _undo.push(cmd);
    std::cout << "[REDO] " << cmd->name() << std::endl;
}

CCommandManager::~CCommandManager()
{
    while (!_undo.empty()) { delete _undo.top(); _undo.pop(); }
    while (!_redo.empty()) { delete _redo.top(); _redo.pop(); }
}
/*
void CCommandManager::beginPreview() {
    if (_preview) { delete _preview; _preview = nullptr; }
}

void CCommandManager::preview(CCommand* cmd) {
    if (!cmd) return;

    if (_preview) {
        _preview->unexecute();
        delete _preview;
        _preview = nullptr;
    }

    cmd->execute();
    _preview = cmd;
}

void CCommandManager::endPreview(CCommand* finalCmd) {
    if (_preview) { delete _preview; _preview = nullptr; }

    execute(finalCmd);
}
*/
