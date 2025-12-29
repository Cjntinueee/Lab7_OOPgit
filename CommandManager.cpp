#include <CommandManager.h>
#include <iostream>

void CCommandManager::execute(CCommand* cmd){
    /*cmd->execute();
    _undo.push(cmd);
    while (!_redo.empty()){
        _redo.pop();
    }*//*
    if (!cmd) return;
    cmd->execute();
    _undo.push(cmd);
    std::cout << "[DO]   " << cmd->name() << std::endl;
    //history.push_back(cmd->name());
    while (!_redo.empty()) { delete _redo.top(); _redo.pop(); }*/
    if (!cmd) return;

    cmd->execute();
    CCommand* snapshot = cmd->clone();
    _undo.push(snapshot);
    std::cout << "[DO]   " << cmd->name() << std::endl;
    while (!_redo.empty()) {
        delete _redo.top();
        _redo.pop();
    }

    delete cmd;

}

void CCommandManager::undo(){
    if (_undo.empty()) return;
    auto cmd = _undo.top();
    _undo.pop();
    cmd->unexecute();
    std::cout << "[UNDO] " << cmd->name() << std::endl;
    _redo.push(cmd);

}
void CCommandManager::redo(){
    if (_redo.empty()) return;
    auto cmd = _redo.top();
    _redo.pop();
    cmd->execute();
    _undo.push(cmd);
    std::cout << "[REDO] " << cmd->name() << std::endl;
}

CCommandManager::~CCommandManager()
{
    while (!_undo.empty()) { delete _undo.top(); _undo.pop(); }
    while (!_redo.empty()) { delete _redo.top(); _redo.pop(); }
}

