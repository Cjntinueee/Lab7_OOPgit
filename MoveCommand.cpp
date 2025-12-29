#include <MoveCommand.h>


CMoveCommand::CMoveCommand(MyStorage* s, int dx, int dy, int w, int h, int t) {
    _storage = s;
    _dx = dx;
    _dy = dy;
    winW = w;
    winH = h;
    token = t;
}

void CMoveCommand::execute() {
    if (!_storage) return;

    if (targets.empty())
        targets = _storage->GetSelectedAll();

    for (auto* f : targets)
        if (f) f->Move(_dx, _dy, winW, winH, true, token);
}

void CMoveCommand::unexecute() {
    for (auto* f : targets) {
        f->Move(-_dx, -_dy, winW, winH, true, token);
    }
}

CCommand* CMoveCommand::clone() {
    auto* c = new CMoveCommand(_storage, _dx, _dy, winW, winH, token);
    c->targets = targets;
    return c;
}

