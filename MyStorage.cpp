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



void MyStorage::SelectOnly(CFigure* obj)
{/*
    if (obj && obj->GetInGroup()) obj = nullptr;
    for (int i = 0; i < count; ++i) {
        figure[i]->SetSel(false);
    }

    if (obj) {
        obj->SetSel(true);
    }*/
    if (obj && obj->GetInGroup()) obj = nullptr;

    for (int i = 0; i < count; ++i)
        figure[i]->SetSel(false);

    SelectedFigures.clear();

    if (obj) {
        obj->SetSel(true);
        SelectedFigures.push_back(obj);
    }

    lastEvent = StorageEvent::SelectionChanged;
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

std::vector<CFigure*> MyStorage::GetSelectedAll() {
    return SelectedFigures;
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

void MyStorage::DelSelection(){
    for (int j = 0; j < count; ++j) {
        if (auto *other = Get(j)) {
            other->ClearSel();
        }
    }
    SelectedFigures.clear();
    lastEvent = StorageEvent::SelectionChanged;
    notify();
}



void MyStorage::Remove(CFigure* f, bool doDelete){
    if (!f) return;

    for (auto it = arrows.begin(); it != arrows.end(); ) {
        CArrow* ar = *it;
        if (!ar) { it = arrows.erase(it); continue; }

        if (ar->Src() == f || ar->Dst() == f || ar->Selected()) {
            delete ar;
            it = arrows.erase(it);
        } else {
            ++it;
        }
    }

    int newCount = 0;
    for (int i = 0; i < count; ++i) {
        if (figure[i] == f) {
        } else {
            figure[newCount++] = figure[i];
        }
    }
    count = newCount;

    for (auto it = SelectedFigures.begin(); it != SelectedFigures.end(); ) {
        if (*it == f) it = SelectedFigures.erase(it);
        else ++it;
    }

    if (doDelete) delete f;

    lastEvent = StorageEvent::StructureChanged;
    notify();
}



void MyStorage::Paint(QPainter *p){
    for (auto* ar : arrows) {
        if (ar) ar->paintt(p);
    }

    for (int i = 0; i < count; i++){
        if (Get(i) != nullptr){
            Get(i)->paintt(p);
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



bool MyStorage::Click(int x, int y, bool ctrl){

    for (int i = (int)arrows.size() - 1; i >= 0; --i) {
        CArrow* ar = arrows[i];
        if (ar && ar->MouseIn(x, y)) {
            if (ctrl) ar->SetSel(!ar->Selected());
            else {
                DelSelection();
                for (auto* a : arrows) if (a) a->SetSel(false);
                ar->SetSel(true);
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
                /*
                f->SetSel(!f->Selected());
                SelectedFigures.push_back(f);*/
                bool newState = !f->Selected();
                f->SetSel(newState);

                auto it = std::find(SelectedFigures.begin(), SelectedFigures.end(), f);
                if (newState) {
                    if (it == SelectedFigures.end()) SelectedFigures.push_back(f);
                } else {
                    if (it != SelectedFigures.end()) SelectedFigures.erase(it);
                }
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
    for (auto* a : arrows) if (a) a->SetSel(false);
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
    /*
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

    return true;*/

    std::ofstream out(s);
    if (!out) return false;

    out << count << '\n';

    for(int i = 0; i < count; i++){
        if (!figure[i]) continue;
        figure[i]->save(out);
    }

    out << arrows.size() << '\n';
    for (auto* ar : arrows) {
        if (!ar) {
            out << -1 << ' ' << -1 << ' ' << 0 << '\n';
            continue;
        }
        if (!ar->Src() || !ar->Dst()) {
            out << -1 << ' ' << -1 << ' ' << ar->IsBidirectional() << '\n';
            continue;
        }
        int aa = -1;
        int bb = -1;

        for (int i = 0; i < count; ++i) {
            if (figure[i] == ar->Src()) aa = i;
            if (figure[i] == ar->Dst()) bb = i;
        }

        out << aa << ' ' << bb << ' ' << ar->IsBidirectional() << '\n';
    }
    return true;
}

bool MyStorage::Load(const std::string& s, CFactory* factory){
    /*std::ifstream in(s);
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


    return true;*/

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
        bool bid;
        in >> aa >> bb >> bid;

        if (aa >= 0 && aa < count && bb >= 0 && bb < count) {
            arrows.push_back(new CArrow(figure[aa], figure[bb], bid));
        }
    }


    return true;
}



void MyStorage::AddArrow(CFigure* a, CFigure* b, bool bid) {
    /*
    if (!a || !b || a == b) return;

    auto* ar = new CArrow(a, b);
    arrows.push_back(ar);

    lastEvent = StorageEvent::StructureChanged;
    notify();*/
    if (!a || !b || a == b) return;
    auto* ar = new CArrow(a, b, bid);
    arrows.push_back(ar);
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

        if (needDelete || ar->Selected()) {
            delete ar;
            it = arrows.erase(it);
        } else {
            ++it;
        }
    }
}

void MyStorage::RemoveArrow(CArrow* ar) {
    if (!ar) return;
    for (auto it = arrows.begin(); it != arrows.end(); ++it) {
        if (*it == ar) {
            delete ar;
            arrows.erase(it);
            lastEvent = StorageEvent::StructureChanged;
            notify();
            return;
        }
    }
}

bool MyStorage::Contains(CFigure* f) const {
    for (int i = 0; i < count; ++i)
        if (figure[i] == f) return true;
    return false;
}


void MyStorage::InsertFigure(CFigure* f) {
    if (!f) return;
    figure[count++] = f;
    lastEvent = StorageEvent::StructureChanged;
    notify();
}

Group* MyStorage::UnGroupSelectedAndTake()
{
    for (int i = 0; i < count; ++i) {
        Group* g = dynamic_cast<Group*>(figure[i]);
        if (!g || !g->Selected()) continue;

        DelArr();

        int cnt = g->GetCountG();

        for (int j = i; j < count - 1; ++j)
            figure[j] = figure[j + 1];
        --count;

        for (int j = count - 1; j >= i; --j)
            figure[j + cnt] = figure[j];

        for (int k = 0; k < cnt; ++k) {
            CFigure* ff = g->FigureAt(k);
            ff->SetSel(false);
            ff->SetInGroup(false);
            figure[i + k] = ff;
        }

        count += cnt;

        g->ClearWithoutDeletingChildren();

        lastEvent = StorageEvent::StructureChanged;
        notify();
        return g;
    }
    return nullptr;
}

CFigure* MyStorage::FindById(int id) const {
    for (int i = 0; i < count; ++i) {
        if (figure[i] && figure[i]->Id() == id)
            return figure[i];
    }
    return nullptr;
}

int indexOfPtr(const std::vector<CFigure*>& v, CFigure* f) {
    for (int i = 0; i < (int)v.size(); ++i)
        if (v[i] == f) return i;
    return -1;
}

int MyStorage::IndexOf(CFigure* f) const {
    for (int i = 0; i < count; ++i)
        if (figure[i] == f) return i;
    return -1;
}

void push_unique(std::vector<CFigure*>& v, CFigure* f) {
    if (!f) return;
    if (std::find(v.begin(), v.end(), f) == v.end())
        v.push_back(f);
}

void MyStorage::Copy(){
    /*
    for (auto* f : clipboard) delete f;
    clipboard.clear();

    for (auto* f : SelectedFigures) {
        if (!f) continue;
        clipboard.push_back(f->clone());
    }*/
    for (auto* f : clipboard) delete f;
    clipboard.clear();
    arrowClipboard.clear();

    std::vector<CFigure*> orig;
    orig.reserve(count);
    for (int i = 0; i < count; ++i) {
        CFigure* f = figure[i];
        if (f && f->Selected())
            orig.push_back(f);
    }
    if (orig.empty()) return;

    clipboard.reserve(orig.size());
    for (auto* f : orig) {
        CFigure* c = f->clone();
        if (!c) continue;
        c->SetSel(false);
        c->SetInGroup(false);
        clipboard.push_back(c);
    }

    if ((int)clipboard.size() != (int)orig.size()) {
        for (auto* f : clipboard) delete f;
        clipboard.clear();
        arrowClipboard.clear();
        return;
    }

    for (auto* ar : arrows) {
        if (!ar || !ar->Src() || !ar->Dst()) continue;

        int ia = indexOfPtr(orig, ar->Src());
        int ib = indexOfPtr(orig, ar->Dst());
        if (ia == -1 || ib == -1) continue;

        arrowClipboard.push_back({ ia, ib, ar->IsBidirectional() });
    }
}

void MyStorage::Cut() {
    Copy();
    Del();
}

/*
void MyStorage::Paste(int dx, int dy) {
    /*
    DelSelection();

    for (auto* f : clipboard) {
        if (!f) continue;

        CFigure* pasted = f->clone();
        pasted->Move(dx, dy, 1000000, 1000000);
        pasted->SetSel(true);

        Add(pasted);
        SelectedFigures.push_back(pasted);
    }

    lastEvent = StorageEvent::StructureChanged;
    notify();
    PasteClipboard(dx, dy, 1000000, 1000000);
}
*/

void MyStorage::InsertAt(int index, CFigure* f) {
    if (!f) return;
    if (index < 0) index = 0;
    if (index > count) index = count;

    for (int j = count; j > index; --j)
        figure[j] = figure[j - 1];

    figure[index] = f;
    ++count;

    lastEvent = StorageEvent::StructureChanged;
    notify();
}



std::vector<CFigure*> MyStorage::Paste(int dx, int dy, int winW, int winH) {
    std::vector<CFigure*> pasted;
    if (clipboard.empty()) return pasted;

    DelSelection();
    for (auto* a : arrows) if (a) a->SetSel(false);

    pasted.reserve(clipboard.size());

    for (auto* proto : clipboard) {
        if (!proto) continue;
        CFigure* f = proto->clone();
        if (!f) continue;

        f->Move(dx, dy, winW, winH);
        f->SetSel(true);

        Add(f);
        SelectedFigures.push_back(f);
        pasted.push_back(f);
    }

    if ((int)pasted.size() == (int)clipboard.size()) {
        for (const auto& ac : arrowClipboard) {
            if (ac.a < 0 || ac.a >= (int)pasted.size()) continue;
            if (ac.b < 0 || ac.b >= (int)pasted.size()) continue;
            AddArrow(pasted[ac.a], pasted[ac.b], ac.bid);
        }
    }

    lastEvent = StorageEvent::StructureChanged;
    notify();
    return pasted;
}

void MyStorage::DetachNoArrows(CFigure* f) {
    if (!f) return;

    int newCount = 0;
    for (int i = 0; i < count; ++i) {
        if (figure[i] != f) figure[newCount++] = figure[i];
    }
    count = newCount;

    for (auto it = SelectedFigures.begin(); it != SelectedFigures.end(); ) {
        if (*it == f) it = SelectedFigures.erase(it);
        else ++it;
    }
}


void MyStorage::SetPenWidthTo(int w)
{
    auto sel = GetSelectedAll();
    for (auto* f : sel) {
        f->SetPenWidth(w);
    }
}

void MyStorage::SetPenColorToSelected(const QColor& c)
{
    auto sel = GetSelectedAll();
    for (auto* f : sel) {
        f->SetPen(c);
    }
}

void MyStorage::SetPenWidthToSelected(int w)
{
    auto sel = GetSelectedAll();
    for (auto* f : sel) {
        f->SetPenWidth(w);
    }
}
