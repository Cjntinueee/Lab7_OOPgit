#include <Group.h>


Group::Group(int size){
    group = new CFigure*[size];
    countG = 0;
}

Group::~Group(){
}

void Group::addtoGr(CFigure* f){
    group[countG++] = f;
}

void Group::paintt(QPainter *p){
    for (int i = 0; i < countG; i++){
        if (group[i] != nullptr){
            group[i]->paintt(p);
        }
    }

    if (!Selected() || countG <= 0){
        return;
    }

    CFigure* f0 = group[0];
    int minX = f0->GetX();
    int minY = f0->GetY();
    int maxX = f0->GetX() + f0->Width();
    int maxY = f0->GetY() + f0->Height();

    for (int i = 1; i < countG; i++){
        CFigure* f = group[i];
        if (!f) continue;

        minX = std::min(minX, f->GetX());
        minY = std::min(minY, f->GetY());
        maxX = std::max(maxX, f->GetX() + f->Width());
        maxY = std::max(maxY, f->GetY() + f->Height());
    }

    QColor BR(0, 120, 215, 60);
    QColor pp(0, 120, 215, 180);

    p->setBrush(QBrush(BR));
    p->setPen(QPen(pp, 2));

    p->drawRect(minX, minY, maxX - minX, maxY - minY);
}


int Group::Width()
{
    if (countG == 0 || group[0] == nullptr)
        return 0;

    int minX = group[0]->GetX();
    int maxX = group[0]->GetX() + group[0]->Width();

    for (int i = 1; i < countG; i++)
    {
        CFigure* f = group[i];
        if (!f) continue;

        minX = std::min(minX, f->GetX());
        maxX = std::max(maxX, f->GetX() + f->Width());
    }

    return maxX - minX;
}

int Group::Height()
{
    if (countG == 0 || group[0] == nullptr)
        return 0;

    int minY = group[0]->GetY();
    int maxY = group[0]->GetY() + group[0]->Height();

    for (int i = 1; i < countG; i++)
    {
        CFigure* f = group[i];
        if (!f) continue;

        minY = std::min(minY, f->GetY());
        maxY = std::max(maxY, f->GetY() + f->Height());
    }

    return maxY - minY;
}

bool Group::MouseIn(int x, int y) {
    for(int i = 0; i < countG; i++){
        CFigure* f = group[i];
        if (f && f->MouseIn(x, y)){
            return true;
        }
    }
    return false;
}

void Group::Move(int dx, int dy, int winW, int winH, bool notifyObs, int token){

    if (countG <= 0) return;

    int minX = group[0]->GetX();
    int minY = group[0]->GetY();
    int maxX = group[0]->GetX() + group[0]->Width();
    int maxY = group[0]->GetY() + group[0]->Height();

    for (int i = 1; i < countG; ++i) {
        CFigure* f = group[i];
        if (!f) continue;
        minX = std::min(minX, f->GetX());
        minY = std::min(minY, f->GetY());
        maxX = std::max(maxX, f->GetX() + f->Width());
        maxY = std::max(maxY, f->GetY() + f->Height());
    }

    if (minX + dx < 0) dx = -minX;
    if (minY + dy < 0) dy = -minY;
    if (maxX + dx > winW) dx = winW - maxX;
    if (maxY + dy > winH) dy = winH - maxY;

    if (dx == 0 && dy == 0) return;

    for (int i = 0; i < countG; ++i) {
        if (group[i]) group[i]->Move(dx, dy, winW, winH, false, token);
    }

    x = minX + dx;
    y = minY + dy;
    lastDx = dx;
    lastDy = dy;
     lastMoveToken = token;

    if (notifyObs) {
        notify();
    }
}


void Group::ClearSel(){
    SetSel(false);
    for (int i = 0; i < countG; i++)
    {
        CFigure* f = group[i];
        if (f) f->ClearSel();
    }
}



int Group::GetCountG(){
    return countG;
}


CFigure* Group::GetFigure(int i){

    if (i < 0 || i >= countG){
        return nullptr;
    }

    CFigure* res = group[i];

    for (int j = i; j < countG - 1; j++){
        group[j] = group[j+1];
    }

    countG--;
    return res;
}


void Group::save(std::ostream& out) const {
    out << Type().toStdString() << '\n' << countG << '\n';
    for (int i = 0; i < countG; ++i)
        group[i]->save(out);
}

void Group::load(std::istream& in, CFactory* factory) {

    int n;
    in >> n;

    group = new CFigure*[n];
    countG = 0;

    for (int i = 0; i < n; ++i) {
        std::string type;
        in >> type;

        CFigure* f =
            factory->create(type);

        f->load(in, factory);
        group[countG++] = f;
    }

    select = false;
}

void Group::SetPen(QColor c)
{
    for (int i = 0; i < countG; ++i) {
        if (group[i])
            group[i]->SetPen(c);
    }
}


CFigure* Group::FigureAt(int i) const {
    if (i < 0 || i >= countG) return nullptr;
    return group[i];
}













