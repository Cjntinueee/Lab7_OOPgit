#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QPainter>
#include <QAction>

#include <QColor>
#include <QColorDialog>
#include <QPalette>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , storage(100)
{
    ui->setupUi(this);

    ui->spinBox->setMaximum(1000);

    ui->treeWidget->setStorage(&storage);

    connect(ui->treeWidget, &CTreeView::objSelected,
            this, &MainWindow::onTreeObjSel);

    props = new PropertiesPanel(this);

    ui->verticalLayoutProps->addWidget(props);

    connect(props, &PropertiesPanel::edited,
            this, &MainWindow::onPropsEdited);

    connect(props, &PropertiesPanel::requestSelectChanged,
            this, &MainWindow::onPropsSelectedChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPropsSelectedChanged(bool selected)
{
    if (!currentFig) return;

    if (selected) {
        storage.SelectOnly(currentFig);
    } else {
        storage.DelSelection();
    }

    syncProps(true);
    update();
}

void MainWindow::onTreeObjSel(CFigure* obj)
{
    //storage.SelectOnly(obj);
    //update();
    storage.SelectOnly(obj);
    //currentFig = obj;
    //updatePropsPanel();
    //props->setObject(currentFig);
    syncProps(true);
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
/*
    if (event->button() != Qt::LeftButton){
        QMainWindow::mousePressEvent(event);
        return;
    }


    const bool ctrl = (event->modifiers() & Qt::ControlModifier);
    bool hit = storage.Click(event->pos().x(), event->pos().y(), ctrl);
    if (hit) {
        isDragging = true;
        isSelecting = false;
        lastpos = event->pos();
    } else {
        isSelecting = true;
        isDragging = false;
        selectStart = event->pos();
        selectEnd = event->pos();
    }
    //
    auto sel = storage.GetSelectedAll();
    currentFig = (sel.size() == 1 ? sel[0] : nullptr);
    props->setObject(currentFig);
    //updatePropsPanel();
    //

    update();*//*
    if (event->button() != Qt::LeftButton){
        QMainWindow::mousePressEvent(event);
        return;
    }


    const bool ctrl = (event->modifiers() & Qt::ControlModifier);
    bool hit = storage.Click(event->pos().x(), event->pos().y(), ctrl);

    if (hit) {
        isDragging = true;
        isSelecting = false;
        lastpos = event->pos();
        dragLastPos = event->pos();
        dragTotalDx = 0;
        dragTotalDy = 0;
    } else {
        isSelecting = true;
        isDragging = false;
        selectStart = event->pos();
        selectEnd = event->pos();
    }

    update();*/

    if (event->button() != Qt::LeftButton){
        QMainWindow::mousePressEvent(event);
        return;
    }

    const bool ctrl = (event->modifiers() & Qt::ControlModifier);
    bool hit = storage.Click(event->pos().x(), event->pos().y(), ctrl);

    syncProps(true);

    if (hit) {
        isDragging = true;
        isSelecting = false;
        lastpos = event->pos();
    } else {
        isSelecting = true;
        isDragging = false;
        selectStart = event->pos();
        selectEnd = event->pos();
    }

    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    if (isDragging) {
        int dx = event->pos().x() - lastpos.x();
        int dy = event->pos().y() - lastpos.y();

        int token = storage.NextMoveToken();
        cmdMgr.execute(new CMoveCommand(&storage, dx, dy, width(), height(), token));
        lastpos = event->pos();
        update();
    } else if (isSelecting) {
        selectEnd = event->pos();
        update();
    }/*
    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        int dx = event->pos().x() - dragLastPos.x();
        int dy = event->pos().y() - dragLastPos.y();

        storage.Moving(dx, dy, width(), height());

        dragTotalDx += dx;
        dragTotalDy += dy;
        dragLastPos = event->pos();
        update();
    }else if (isSelecting) {
        selectEnd = event->pos();
        update();
    }*/
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {

    const bool shift = (event->modifiers() & Qt::ShiftModifier);

    if (event->button() != Qt::LeftButton)
        return;

    if (isDragging) {

        isDragging = false;
        return;
        /*isDragging = false;

        if (dragTotalDx != 0 || dragTotalDy != 0) {
            cmdMgr.execute(new CMoveCommand(&storage, dragTotalDx, dragTotalDy, width(), height()));
        }
        update();
        return;*/
    }

    if (isSelecting) {
        isSelecting = false;
        selectEnd = event->pos();
        update();

        if (selectEnd.x() < selectStart.x()){
            int vr_x1 = selectStart.x();
            selectStart.setX(selectEnd.x());
            selectEnd.setX(vr_x1);
        }

        if (selectEnd.y() < selectStart.y()){
            int vr_y1 = selectStart.y();
            selectStart.setY(selectEnd.y());
            selectEnd.setY(vr_y1);
        }

        int w = std::abs(selectEnd.x() - selectStart.x());
        int h = std::abs(selectEnd.y() - selectStart.y());

        const int minn = 5;
        if (w < minn && h < minn) {
            storage.DelSelection();
            update();
            return;
        }
        if (cur_figure == F_NONE) {
            return;
        }
        switch(cur_figure){
        case F_EL:
        {
            int rad = std::abs(selectEnd.y() - selectStart.y()) / 2;
            if (shift){
                auto *c = new CEllipse(selectStart.x(), selectStart.y(), rad, rad);
                c->SetPen(curcolp);
                cmdMgr.execute(new CAddCommand(c, &storage));
            } else{
                auto *ce = new CEllipse(selectStart.x(), selectStart.y(), w / 2, h / 2);
                ce->SetPen(curcolp);
                cmdMgr.execute(new CAddCommand(ce, &storage));
            }
            break;
        }
        case F_RECT:
        {

            int side = std::abs(selectEnd.x() - selectStart.x());
            if (shift){
                auto *cs = new CRect(selectStart.x(), selectStart.y(), side, side);
                cs->SetPen(curcolp);
                cmdMgr.execute(new CAddCommand(cs, &storage));
            } else{
                auto *cr = new CRect(selectStart.x(), selectStart.y(), w, h);
                cr->SetPen(curcolp);
                cmdMgr.execute(new CAddCommand(cr, &storage));
            }
            break;
        }
        case F_TRI: {
            CTriangle *t = new CTriangle(selectStart, selectEnd);
            t->SetPen(curcolp);
            cmdMgr.execute(new CAddCommand(t, &storage));
            break;
        }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event){

    QPainter p(this);
    storage.Paint(&p);

    if (isSelecting) {
        QRect rect(selectStart, selectEnd);

        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(1);
        pen.setColor(Qt::white);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);

        p.drawRect(rect);
        storage.Paint(&p);
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Delete){
        cmdMgr.execute(new CDeleteCommand(&storage));
        //updatePropsPanel();
        update();
    }
/*
    if (event->key() == Qt::Key_G){
        cmdMgr.execute(new CGroupCommand(&storage));
        update();
    }

    if (event->key() == Qt::Key_U){
        storage.UnGrouping();
        update();
    }    if (event->key() == Qt::Key_A) {
        auto sel = storage.GetSelectedAll();
        if (sel.size() == 2) {
            storage.AddArrow(sel[0], sel[1]);
            update();
        }
    }*/
    if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Z) {
        cmdMgr.undo();
        syncProps(true);
        //updatePropsPanel();
        update();
        return;
    }

    if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Y) {
        cmdMgr.redo();
        syncProps(true);
        //updatePropsPanel();
        update();
        return;
    }
    if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_C) {
        storage.Copy();
        syncProps(true);
        //updatePropsPanel();
        return;
    }

    if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_X) {
        storage.Copy();
        cmdMgr.execute(new CDeleteCommand(&storage));
        syncProps(true);
        //updatePropsPanel();
        update();
        return;
    }

    if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_V) {
        //cmdMgr.execute(new CPasteCommand(&storage, 20, 20, width(), height()));
        storage.Paste(20, 20, width(), height());
        syncProps(true);
        //updatePropsPanel();
        update();
        return;
    }

}



void MainWindow::on_pb_ell_clicked()
{
    cur_figure = F_EL;
}


void MainWindow::on_pb_rect_clicked()
{
    cur_figure = F_RECT;
}

void MainWindow::on_pb_tri_clicked()
{
    cur_figure = F_TRI;
}


void MainWindow::on_pushButton_color_clicked()
{

    QColor c = QColorDialog::getColor(curcolp, this, "Выберите цвет");

    if (!c.isValid())
        return;

    curcolp = c;


    storage.SetColorTo(curcolp);

    update();
}


void MainWindow::on_spinBox_editingFinished()
{
    storage.SizeCh(ui->spinBox->value(), width(), height());
    update();
}





void MainWindow::on_pushButton_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить",
        "",
        "Text files (*.txt)"
        );

    if (fileName.isEmpty())
        return;

    storage.Save(fileName.toStdString());
    syncProps(true);
}



void MainWindow::on_pushButton_load_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Загрузить",
        "",
        "Text files (*.txt)"
        );

    if (fileName.isEmpty())
        return;

    storage.Load(fileName.toStdString(), &factory);
    syncProps(true);
    update();
}



void MainWindow::on_pushButton_gr_clicked()
{
    cmdMgr.execute(new CGroupCommand(&storage));
    //updatePropsPanel();
    syncProps(true);
    update();
}

void MainWindow::on_pushButton_un_gr_clicked()
{
    cmdMgr.execute(new CUnGroupCommand(&storage));
    //updatePropsPanel();
    syncProps(true);
    update();
}



void MainWindow::on_pushButton_clicked()
{
    auto sel = storage.GetSelectedAll();
    if (sel.size() == 2) {
        storage.AddArrow(sel[0], sel[1], false);
        syncProps(true);
        update();
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    auto sel = storage.GetSelectedAll();
    if (sel.size() == 2) {
        storage.AddArrow(sel[0], sel[1], true);
        syncProps(true);
        update();
    }
}


void MainWindow::syncProps(bool keepLastIfNone)
{/*
    auto sel = storage.GetSelectedAll();

    if (sel.size() == 1) {
        currentFig = sel[0];
        props->setObject(currentFig);
        return;
    }

    if (sel.empty()) {
        if (!keepLastIfNone) {
            currentFig = nullptr;
            props->setObject(nullptr);
        }
        // keepLastIfNone==true -> оставляем как есть (панель показывает последний объект)
        return;
    }*/
    auto sel = storage.GetSelectedAll();

    if (sel.size() == 1) {
        currentFig = sel[0];
        props->setObject(currentFig);
        return;
    }

    if (sel.empty()) {
        currentFig = nullptr;
        props->setObject(nullptr);
        return;
    }

    currentFig = nullptr;
    props->setObject(nullptr);

}
