#include <DeleteCommand.h>


CDeleteCommand::CDeleteCommand(MyStorage* storage) {
    _storage = storage;
}

void CDeleteCommand::execute() {/*
    removed = _storage->GetSelectedAll();
    if (removed.empty()) return;
    for (auto* f : removed) {
        if (!f) continue;
        _storage->Remove(f, false);
    }
    _storage->DelSelection();*/
    removedFigures.clear();
    removedArrows.clear();

    removedFigures = _storage->GetSelectedAll();

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

void CDeleteCommand::unexecute() {/*
    for (auto* f : removed) {
        if (!f) continue;
        _storage->Add(f);
    }
    removed.clear();*/
    for (auto* f : removedFigures) {
        if (!f) continue;
        _storage->Add(f);
    }

    for (auto& info : removedArrows) {
        if (!info.a || !info.b) continue;
        _storage->AddArrow(info.a, info.b, info.bid);
    }

    removedFigures.clear();
    removedArrows.clear();
}

CCommand* CDeleteCommand::clone() {
    return new CDeleteCommand(_storage);
}
