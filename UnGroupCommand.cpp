#include <UnGroupCommand.h>

CUnGroupCommand::CUnGroupCommand(MyStorage* s)
{
    _storage = s;
}

void CUnGroupCommand::execute()
{/*
    if (!_storage) return;

    CFigure* sel = _storage->GetSelected();
    _group = dynamic_cast<Group*>(sel);
    if (!_group) return;

    members.clear();
    for (int i = 0; i < _group->GetCountG(); ++i) {
        members.push_back(_group->FigureAt(i));
    }

    _storage->DelSelection();
    _group->SetSel(true);

    _storage->UnGroupSelectedAndTake();*/
    /*
    if (!_storage) return;

    CFigure* sel = _storage->GetSelected();
    _group = dynamic_cast<Group*>(sel);
    if (!_group) return;

    groupIndex = _storage->IndexOf(_group);

    members.clear();
    for (int i = 0; i < _group->GetCountG(); ++i)
        members.push_back(_group->FigureAt(i));

    _storage->DelSelection();
    _group->SetSel(true);

    _storage->UnGroupSelectedAndTake();*/
    if (!_storage) return;

    if (!captured) {
        CFigure* sel = _storage->GetSelected();
        _group = dynamic_cast<Group*>(sel);
        if (!_group) return;

        groupIndex = _storage->IndexOf(_group);

        members.clear();
        for (int i = 0; i < _group->GetCountG(); ++i)
            members.push_back(_group->FigureAt(i));

        captured = true;
    } else {
        _storage->DelSelection();
        _storage->SelectOnly(_group);
    }

    _storage->UnGroupSelected();
}

void CUnGroupCommand::unexecute()
{/*
    if (!_storage || !_group) return;

    _storage->DelSelection();

    for (auto* f : members) {
        f->SetSel(true);
    }

    _storage->Grouping();*/
    if (!_storage || !_group) return;

    for (auto* f : members) {
        if (!f) continue;
        _storage->DetachNoArrows(f);
        f->SetInGroup(true);
        f->SetSel(false);
        _group->addtoGr(f);
    }

    _group->SetSel(true);
    _storage->InsertAt(groupIndex, _group);

    _storage->lastEvent = MyStorage::StorageEvent::StructureChanged;
    _storage->notify();
}

CCommand* CUnGroupCommand::clone()
{
    //return new CUnGroupCommand(_storage);
    auto* c = new CUnGroupCommand(_storage);
    c->_group = _group;
    c->members = members;
    c->groupIndex = groupIndex;
    c->captured = captured;
    return c;
}
