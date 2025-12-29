#include <AddCommand.h>


CAddCommand::CAddCommand(CFigure* f, MyStorage* storage) {
    _f = f;
    _storage = storage;
}

void CAddCommand::execute() {
    if (_f != nullptr){
        _storage->Add(_f);
    }

    for (auto &info : removedArrows) {
        if (info.a && info.b)
            _storage->AddArrow(info.a, info.b, info.bid);
    }
}

void CAddCommand::unexecute() {
    /*if (_f != nullptr){
        _f->SetSel(false);
        _storage->Remove(_f, false);
    }*/
    if (!_storage || !_f) return;

    if (!captured) {
        removedArrows.clear();
        auto &ars = _storage->GetArrows();
        for (auto* ar : ars) {
            if (!ar) continue;
            if (ar->Src() == _f || ar->Dst() == _f) {
                removedArrows.push_back({ ar->Src(), ar->Dst(), ar->IsBidirectional() });
            }
        }
        captured = true;
    }

    _f->SetSel(true);
    _storage->Remove(_f, false);
}

CCommand* CAddCommand::clone() {
    return new CAddCommand(_f, _storage);
}
