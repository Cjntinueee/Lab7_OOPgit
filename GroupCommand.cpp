#include <GroupCommand.h>

CGroupCommand::CGroupCommand(MyStorage* storage){
    _storage = storage;
}

void CGroupCommand::execute() {

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
        created->ClearCount();
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

    Group* g = _storage->UnGroupSelected();
    if (!g) return;

    created = g;

    _storage->DelSelection();
}

CCommand* CGroupCommand::clone() {
    auto* c = new CGroupCommand(_storage);
    c->grouped = grouped;
    c->created = created;
    c->indices = indices;
    c->groupIndex = groupIndex;
    return c;
}
