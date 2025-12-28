#include <GroupCommand.h>

CGroupCommand::CGroupCommand(MyStorage* storage){
    _storage = storage;
}

void CGroupCommand::execute() {
    if (!_storage) return;
    _storage->Grouping();
    created = dynamic_cast<Group*>(_storage->Get(_storage->GetCount() - 1));
}

void CGroupCommand::unexecute() {
    if (!_storage || !created) return;
    _storage->DelSelection();
    created->SetSel(true);
    _storage->UnGrouping();
    created = nullptr;
}

CCommand* CGroupCommand::clone() {
    return new CGroupCommand(_storage);
}
