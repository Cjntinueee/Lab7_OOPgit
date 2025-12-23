#include <TreeView.h>

CTreeView::CTreeView(QWidget *parent): QTreeWidget(parent){

    setColumnCount(1);
    setHeaderHidden(true);

    connect(this, &QTreeWidget::itemClicked,
            this, &CTreeView::click_on_item);
}
void CTreeView::OnSubjChanged(CObject* who) {
    if(who != stor || !stor) {
        return;
    }
    if (stor->GetLastEvent() == MyStorage::StorageEvent::StructureChanged) {
        rebuild();
        synchronStorage();
    }
    else if (stor->GetLastEvent() == MyStorage::StorageEvent::SelectionChanged) {
        synchronStorage();
    }
}


void CTreeView::click_on_item(QTreeWidgetItem* it, int){

    if (!it) return;

    QVariant v = it->data(0, Qt::UserRole);
    if (!v.isValid()) return;

    CFigure* obj = reinterpret_cast<CFigure*>(v.value<quintptr>());
    if (!obj) return;
    emit objSelected(obj);


}



void CTreeView::processNode(QTreeWidgetItem* parent, CFigure* obj)
{
    if (!obj) return;

    QTreeWidgetItem* node = new QTreeWidgetItem(parent); //
    node->setText(0, obj->Type());
    node->setData(0, Qt::UserRole, QVariant::fromValue((quintptr)obj)); ///

    if (obj->Type() == "G") {
        if (auto* g = dynamic_cast<Group*>(obj)) { //
            for (int k = 0; k < g->GetCountG(); ++k)
                processNode(node, g->FigureAt(k));
        }
        node->setExpanded(true); ///
    }
}



void CTreeView::setStorage(MyStorage* s)
{
    if (stor == s) return;

    stor = s;
    if (stor) stor->addObs(this);

    rebuild();
    synchronStorage();
}


void CTreeView::rebuild()
{
    if (!stor) return;

    suppress = true; //
    clear();

    QTreeWidgetItem* root = new QTreeWidgetItem(this); //
    root->setText(0, "Storage"); //
    root->setExpanded(true); ///

    for (int i = 0; i < stor->GetCount(); ++i)
        processNode(root, stor->Get(i));

    suppress = false; //
}


void CTreeView::synchronStorage()
{
    if (!stor) return;

    suppress = true;
    clearSelection();

    CFigure* sel = stor->GetSelected();
    if (sel) {
        auto all = findItems("*", Qt::MatchWildcard | Qt::MatchRecursive, 0); ////
        for (auto* it : all) {  // переписать
            QVariant v = it->data(0, Qt::UserRole); ///
            if (!v.isValid()) continue;

            CFigure* f = reinterpret_cast<CFigure*>(v.value<quintptr>());
            if (f == sel && !f->GetInGroup()) { ///
                it->setSelected(true);
                setCurrentItem(it); //
                break;
            }
        }
    }

    suppress = false;
}























