#include <GroupCommand.h>

CGroupCommand::CGroupCommand(MyStorage* storage){
    _storage = storage;
}

void CGroupCommand::execute() {

    /*if (!_storage) return;

    // первый запуск: запоминаем детей и их индексы
    if (grouped.empty()) {
        grouped = _storage->GetSelectedAll();
        if (grouped.size() < 2) return;

        indices.clear();
        indices.reserve(grouped.size());
        for (auto* f : grouped)
            indices.push_back(_storage->IndexOf(f));

        groupIndex = *std::min_element(indices.begin(), indices.end());
        if (groupIndex < 0) groupIndex = _storage->GetCount();
    }

    // redo: если группа уже была создана — используем её же
    if (!created) {
        created = new Group((int)grouped.size());
        for (auto* f : grouped) {
            created->addtoGr(f);
            f->SetInGroup(true);
            f->SetSel(false);
        }
        created->SetSel(false);
    }

    // вынуть детей из storage (НЕ delete), как в DeleteCommand
    for (auto* f : grouped) {
        if (!f) continue;
        _storage->Remove(f, false);
    }

    // вставить группу в нужное место
    _storage->InsertAt(groupIndex, created);

    _storage->DelSelection();
    created->ClearWithoutDeletingChildren();
    for (auto* f : grouped) {
        created->addtoGr(f);
        f->SetInGroup(true);
    }*/
    if (!_storage) return;

    if (grouped.empty()) {
        grouped = _storage->GetSelectedAll();
        if (grouped.size() < 2) return;

        indices.clear();
        for (auto* f : grouped) indices.push_back(_storage->IndexOf(f));
        groupIndex = *std::min_element(indices.begin(), indices.end());
        if (groupIndex < 0) groupIndex = _storage->GetCount();
    }

    if (!created) {
        created = new Group((int)grouped.size());
    } else {
        created->ClearWithoutDeletingChildren();
    }

    for (auto* f : grouped) {
        created->addtoGr(f);
        if (f) {
            f->SetInGroup(true);
            f->SetSel(false);
        }
    }
    created->SetSel(false);

    for (auto* f : grouped) {
        if (f) _storage->Remove(f, false);
    }

    _storage->InsertAt(groupIndex, created);

    _storage->DelSelection();
}

void CGroupCommand::unexecute() {

    if (!_storage || !created) return;

    _storage->DelSelection();
    created->SetSel(true);

    Group* g = _storage->UnGroupSelectedAndTake();
    if (!g) return;

    created = g;

    _storage->DelSelection();
}

CCommand* CGroupCommand::clone() {
    return new CGroupCommand(_storage);
}
