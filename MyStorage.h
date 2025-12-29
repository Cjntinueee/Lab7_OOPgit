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

    std::vector<CFigure*> clipboard;

    struct ArrowClip {
        int a;
        int b;
        bool bid;
    };
    std::vector<ArrowClip> arrowClipboard;

public:

    MyStorage();
    MyStorage(int size);
    ~MyStorage();

    void Add(CFigure* C);

    void SelectOnly(CFigure* obj);

    CFigure* Get(int ind);
    CFigure* GetSelected();
    int GetCount();
    std::vector<CFigure*> GetSelectedAll();

    void Del();
    void DelSelection();

    void Remove(CFigure* f, bool doDelete);

    void Paint(QPainter *p);
    void SetColorTo(QColor c);

    bool Click(int x, int y, bool ctrl);
    void Moving(int dx, int dy, int winW, int winH);
    void SizeCh(int size, int winW, int winH);

    void Grouping();
    void UnGrouping();

    bool Save(const std::string& s);
    bool Load(const std::string& s, CFactory* factory);

    enum class StorageEvent {
        None,
        StructureChanged,
        SelectionChanged
    };

    StorageEvent lastEvent = StorageEvent::None;
    StorageEvent GetLastEvent() const { return lastEvent; }

    void AddArrow(CFigure* a, CFigure* b, bool bid);

    void DelArr();
    int NextMoveToken() { return ++moveTokenCounter; }

    const std::vector<CArrow*>& GetArrows() const { return arrows; }
    void RemoveArrow(CArrow* ar);

    //bool Contains(CFigure* f) const;

    void Copy();
    void Cut();
    void Paste(int dx = 20, int dy = 20);

    Group* UnGroupSelected();
    void InsertFigure(CFigure* f);

    CFigure* FindById(int id) const;
    void InsertAt(int index, CFigure* f);
    int IndexOf(CFigure* f) const;

    std::vector<CFigure*> PasteClipboard(int dx, int dy, int winW, int winH);
    void DetachNoArrows(CFigure* f);

    void SetPenWidthTo(int w);

    void SetPenColorToSelected(const QColor& c);
    void SetPenWidthToSelected(int w);

    void DeleteSelectedNoFree();


    //void AddArrow(CArrow* ar);
    //int  ArrowIndexOf(CArrow* ar) const;
    //void RemoveArrowNoDelete(CArrow* ar);
    //void InsertArrowAt(int idx, CArrow* ar);
};

#endif // MYSTORAGE_H
