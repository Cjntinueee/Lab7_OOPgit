#include <DeleteCommand.h>


CDeleteCommand::CDeleteCommand(MyStorage* storage) {
    _storage = storage;
}

void CDeleteCommand::execute() {
    if (!_storage) return;

    if (!captured) {
        removedFigures = _storage->GetSelectedAll();

        removedArrows.clear();
        auto& ars = _storage->GetArrows();
        for (auto* ar : ars) {
            if (!ar) continue;

            bool needDelete = ar->Selected();

            if (!needDelete && !removedFigures.empty()) {
                for (auto* f : removedFigures) {
                    if (!f) continue;
                    if (ar->Src() == f || ar->Dst() == f) { needDelete = true; break; }
                }
            }

            if (needDelete) {
                removedArrows.push_back({ ar->Src(), ar->Dst(), ar->IsBidirectional() });
            }
        }

        if (removedFigures.empty() && removedArrows.empty()) return;
        captured = true;
    }

    for (auto& info : removedArrows) {
        auto current = _storage->GetArrows();
        for (auto* ar : current) {
            if (!ar) continue;
            if (ar->Src() == info.a && ar->Dst() == info.b && ar->IsBidirectional() == info.bid) {
                _storage->RemoveArrow(ar);
                break;
            }
        }
    }

    for (auto* f : removedFigures) {
        if (!f) continue;
        _storage->Remove(f, false);
    }

    _storage->DelSelection();
}

void CDeleteCommand::unexecute() {

    for (auto* f : removedFigures) {
        if (!f) continue;
        _storage->Add(f);
    }

    for (auto& info : removedArrows) {
        if (!info.a || !info.b) continue;
        _storage->AddArrow(info.a, info.b, info.bid);
    }

}

CCommand* CDeleteCommand::clone() {
    auto* c = new CDeleteCommand(_storage);
    c->removedFigures = removedFigures;
    c->removedArrows = removedArrows;
    c->captured = captured;
    return c;
}
