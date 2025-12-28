#include <AddCommand.h>


CAddCommand::CAddCommand(CFigure* f, MyStorage* storage) {
    _f = f;
    _storage = storage;
}

void CAddCommand::execute() {
    if (_f != nullptr){
        _storage->Add(_f);
    }
}

void CAddCommand::unexecute() {
    if (_f != nullptr){
        _f->SetSel(true);
        _storage->Remove(_f, false);
    }
}

CCommand* CAddCommand::clone() {
    return new CAddCommand(_f, _storage);
}
