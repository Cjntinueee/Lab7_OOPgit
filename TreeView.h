#ifndef TREEVIEW_H
#define TREEVIEW_H
#include <QTreeWidget>
#include <Observer.h>
#include <QKeyEvent>
#include <Figures.h>
#include <MyStorage.h>
/*
class CTreeView: public QTreeWidget, public CObserver {
private:
    Q_OBJECT

    MyStorage* stor = nullptr;
    bool suppress = false;

    void rebuild();
    void synchronStorage();
    void processNode(QTreeWidgetItem* parent, CFigure* obj);
public:
    CTreeView(QWidget *parent = nullptr);
    ~CTreeView() {stor->removeObs(this);}
    void setStorage(MyStorage* s);
    void OnSubjChanged(CObject* who) override;

signals:
    void objSelected(CFigure* f);

private slots:
    void click_on_item(QTreeWidgetItem* it, int);

};
*/
class CTreeView: public QTreeWidget, public CObserver {
private:
    Q_OBJECT

    MyStorage* stor = nullptr;
    bool suppress = false;

    void rebuild();
    void synchronStorage();
    void processNode(QTreeWidgetItem* parent, CFigure* obj);
public:
    CTreeView(QWidget *parent = nullptr);
    ~CTreeView() {stor->removeObs(this);}
    void setStorage(MyStorage* s);
    void OnSubjChanged(CObject* who) override;


signals:
    void objSelected(CFigure* f);
    //void objsSelected(const std::vector<CFigure*>& objs);

private slots:
    void click_on_item(QTreeWidgetItem* it, int);
    //void onSelectionChanged();

};
#endif // TREEVIEW_H
