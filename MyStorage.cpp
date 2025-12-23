#include <Figures.h>
#include <MyStorage.h>
#include <QPaintEvent>
#include <fstream>
#include <Arrow.h>


MyStorage::MyStorage() {
    figure = nullptr;
    count = 0;
}

MyStorage::MyStorage(int size)
{
    figure = new CFigure*[size];
    count = 0;
}

MyStorage::~MyStorage()
{
    for (int i = 0; i < count; i++){
        delete figure[i];
    }
    delete[] figure;
    figure = nullptr;
}

void MyStorage::Add(CFigure* C)
{
    figure[count++] = C;
    lastEvent = StorageEvent::StructureChanged;
    notify();
}

CFigure* MyStorage::Get(int ind)
{
    if (ind < 0 || ind >= count) return nullptr;
    return figure[ind];
}

CFigure* MyStorage::GetSelected(){
    for (int i = 0; i < count; ++i) {
        if (figure[i]->Selected()) {
            return figure[i];
        }
    }
    return nullptr;
}

int MyStorage::GetCount()
{
    return count;
}

void MyStorage::Del()
{
    DelArr();

    int newCount = 0;
    for (int i = 0; i < count; i++)
    {
        CFigure* ff = figure[i];
        if (ff && ff->Selected()){
            delete ff;
        } else{
            figure[newCount++] = ff;
        }
    }
    count = newCount;
    lastEvent = StorageEvent::StructureChanged;
    notify();
}

void MyStorage::DelArr(){
    std::vector<CFigure*> toDel;
    for (int i = 0; i < count; i++) {
        if (figure[i] && figure[i]->Selected())
            toDel.push_back(figure[i]);
    }

    for (auto it = arrows.begin(); it != arrows.end(); ) {
        CArrow* ar = *it;

        if (ar == nullptr) {
            it = arrows.erase(it);
            continue;
        }

        bool needDelete = false;

        for (auto itF = toDel.begin(); itF != toDel.end(); ++itF) {
            auto* f = *itF;
            if (ar->Src() == f || ar->Dst() == f) {
                needDelete = true;
                break;
            }
        }

        if (needDelete || ar->GetSelA()) {
            delete ar;
            it = arrows.erase(it);
        } else {
            ++it;
        }
    }
}


void MyStorage::DelSelection(){
    for (int j = 0; j < count; ++j) {
        if (auto *other = Get(j)) {
            other->ClearSel();
        }
    }
}

void MyStorage::Paint(QPainter *p){
    for (auto* ar : arrows) {
        if (ar) ar->paintA(p);
    }

    for (int i = 0; i < count; i++){
        if (Get(i) != nullptr){
            Get(i)->paintt(p);
        }

    }
}

bool MyStorage::Click(int x, int y, bool ctrl){
/*
    for (int i = 0; i < count; i++){

        CFigure* f = Get(i);
        if (f->MouseIn(x, y)){
            if (ctrl){
                f->SetSel(!f->Selected());
            } else{
                DelSelection();
                f->SetSel(true);
            }
            lastEvent = StorageEvent::SelectionChanged;
            notify();
            return true;
        }

    }
    DelSelection();
    //lastEvent = StorageEvent::SelectionChanged;
    //notify();
    return false;*/

    for (int i = (int)arrows.size() - 1; i >= 0; --i) {
        CArrow* ar = arrows[i];
        if (ar && ar->MouseInA(x, y)) {
            if (ctrl) ar->SetSelA(!ar->GetSelA());
            else {
                DelSelection();
                for (auto* a : arrows) if (a) a->SetSelA(false);
                ar->SetSelA(true);
            }
            lastEvent = StorageEvent::SelectionChanged;
            notify();
            return true;
        }
    }
    for (int i = 0; i < count; i++){

        CFigure* f = Get(i);
        if (f->MouseIn(x, y)){
            if (ctrl){
                f->SetSel(!f->Selected());
                SelectedFigures.push_back(f);
            } else{
                DelSelection();
                SelectedFigures.clear();
                f->SetSel(true);
                SelectedFigures.push_back(f);
            }
            lastEvent = StorageEvent::SelectionChanged;
            notify();
            return true;
        }

    }
    DelSelection();
    for (auto* a : arrows) if (a) a->SetSelA(false);
    SelectedFigures.clear();
    lastEvent = StorageEvent::SelectionChanged;
    notify();
    return false;
}


void MyStorage::Moving(int dx, int dy, int winW, int winH){
    int token = ++moveTokenCounter;

    for (auto* ar : arrows) {
        if (ar) ar->SetViewport(winW, winH);
    }
    for (int i = 0; i < count; i++){

        CFigure* f = Get(i);
        if (f->Selected()){
            f->Move(dx, dy, winW, winH, true, token);
        }
    }
}


void MyStorage::SizeCh(int size, int winW, int winH){
    for (int i = 0; i < count; i++){

        CFigure* f = Get(i);
        if (f->Selected()){
            f->SizeChange(size, winW, winH);
        }
    }
}


void MyStorage::SetColorTo(QColor c) {
    for (int i = 0; i < count; ++i) {
        CFigure *f = Get(i);
        if (f && f->Selected()) {
            f->SetPen(c);
        }
    }
}



void MyStorage::Grouping(){

    int selectedCount = 0;
    for (int i = 0; i < count; i++)
        if (figure[i] && figure[i]->Selected())
            selectedCount++;

    if (selectedCount < 2)
        return;

    Group* g = new Group(selectedCount);

    int newCount = 0;
    for (int i = 0; i < count; i++){
        CFigure *f = Get(i);
        if (f && f->Selected()){
            g->addtoGr(f);
            f->SetInGroup(true);
        }
        else {
            figure[newCount++] = f;
        }
    }

    count = newCount;
    g->SetSel(false);
    Add(g);
}

void MyStorage::UnGrouping(){
    for (int i = 0; i < count; i++){

        Group* g = dynamic_cast<Group*>(figure[i]);
        if (!g || !g->Selected()){
            continue;
        }
        DelArr();

        int cnt = g->GetCountG();

        for (int j = count - 1; j > i; j--){
            figure[j + (cnt - 1)] = figure[j];
        }

        for(int k = 0; k < cnt; k++){
            CFigure* ff = g->GetFigure(0);
            ff->SetSel(false);
            figure[i+k] = ff;
            ff->SetInGroup(false);
        }
        delete g;
        count += (cnt - 1);
        i += (cnt - 1);
    }
    lastEvent = StorageEvent::StructureChanged;
    notify();
}

bool MyStorage::Save(const std::string& s){
    std::ofstream out(s);
    if (!out) return false;

    out << count << '\n';

    for(int i = 0; i < count; i++){
        figure[i]->save(out);
    }

    out << arrows.size() << '\n';
    for (auto* ar : arrows) {
        int aa = -1;
        int bb = -1;

        for (int i = 0; i < count; ++i) {
            if (figure[i] == ar->Src()) aa = i;
            if (figure[i] == ar->Dst()) bb = i;
        }

        out << aa << ' ' << bb << '\n';
    }

    return true;
}

bool MyStorage::Load(const std::string& s, CFactory* factory){
    std::ifstream in(s);
    if (!in) return false;

    int n;
    in >> n;

    for (int i = 0; i < count; ++i)
        delete figure[i];
    count = 0;

    for (auto* ar : arrows)
        delete ar;
    arrows.clear();

    for (int i = 0; i < n; ++i){
        std::string type;
        in >> type;

        CFigure* obj = factory->create(type);

        obj->load(in, factory);
        Add(obj);
    }

    int na;
    in >> na;

    for (int i = 0; i < na; ++i){
        int aa, bb;
        in >> aa >> bb;

        if (aa >= 0 && aa < count && bb >= 0 && bb < count) {
            arrows.push_back(new CArrow(figure[aa], figure[bb]));
        }
    }


    return true;
}


void MyStorage::SelectOnly(CFigure* obj)
{
    if (obj && obj->GetInGroup()) obj = nullptr;
    for (int i = 0; i < count; ++i) {
        figure[i]->SetSel(false);
    }

    if (obj) {
        obj->SetSel(true);
    }

    //lastEvent = StorageEvent::SelectionChanged;
    //notify();
}


void MyStorage::AddArrow(CFigure* a, CFigure* b)
{
    if (!a || !b || a == b) return;

    auto* ar = new CArrow(a, b);
    arrows.push_back(ar);

    lastEvent = StorageEvent::StructureChanged;
    notify();
}


std::vector<CFigure*> MyStorage::GetSelectedAll() {
    return SelectedFigures;
}





