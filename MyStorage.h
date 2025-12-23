#ifndef MYSTORAGE_H
#define MYSTORAGE_H

#include <Figures.h>
#include <Object.h>
#include <Group.h>
#include <QPaintEvent>
#include <Arrow.h>
#include <vector>

class MyStorage: public CObject
{
private:
    CFigure** figure;
    int count;

    std::vector<CArrow*> arrows;
    int moveTokenCounter = 0;

    std::vector<CFigure*> SelectedFigures;
public:

    MyStorage();
    MyStorage(int size);
    ~MyStorage();

    void Add(CFigure* C);
    CFigure* Get(int ind);
    CFigure* GetSelected();

    int GetCount();

    void Del();
    void DelSelection();

    void Paint(QPainter *p);
    bool Click(int x, int y, bool ctrl);
    void Moving(int dx, int dy, int winW, int winH);

    void SizeCh(int size, int winW, int winH);

    void SetColorTo(QColor c);

    void Grouping();
    void UnGrouping();

    bool Save(const std::string& s);
    bool Load(const std::string& s, CFactory* factory);

    void SelectOnly(CFigure* obj);

    enum class StorageEvent {
        None,
        StructureChanged,
        SelectionChanged
    };

    StorageEvent lastEvent = StorageEvent::None;
    StorageEvent GetLastEvent() const { return lastEvent; }
    void ApplyTreeSelection(const std::vector<CFigure*>& sel);

    void AddArrow(CFigure* a, CFigure* b);
    std::vector<CFigure*> GetSelectedAll();
    void DelArr();

};

#endif // MYSTORAGE_H
