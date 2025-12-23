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
    //ui->spinBox->setFocusPolicy(Qt::NoFocus);

    connect(ui->treeWidget, &CTreeView::objSelected,
            this, &MainWindow::onTreeObjSel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTreeObjSel(CFigure* obj)
{
    storage.SelectOnly(obj);
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event){

    if (event->button() != Qt::LeftButton){
        QMainWindow::mousePressEvent(event);
        return;
    }


    if (isClickOnUi(event->pos())) {
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

    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    if (isDragging) {
        int dx = event->pos().x() - lastpos.x();
        int dy = event->pos().y() - lastpos.y();

        storage.Moving(dx, dy, width(), height());
        lastpos = event->pos();
        update();
    } else if (isSelecting) {
        selectEnd = event->pos();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {

    const bool shift = (event->modifiers() & Qt::ShiftModifier);

    if (event->button() != Qt::LeftButton)
        return;

    if (isClickOnUi(event->pos())) {
        QMainWindow::mouseReleaseEvent(event);
        return;
    }

    if (isDragging) {
        isDragging = false;
        return;
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
                storage.Add(c);
            } else{
                auto *ce = new CEllipse(selectStart.x(), selectStart.y(), w / 2, h / 2);
                ce->SetPen(curcolp);
                storage.Add(ce);
            }
            break;
        }
        case F_RECT:
        {

            int side = std::abs(selectEnd.x() - selectStart.x());
            if (shift){
                auto *cs = new CRect(selectStart.x(), selectStart.y(), side, side);
                cs->SetPen(curcolp);
                storage.Add(cs);
            } else{
                auto *cr = new CRect(selectStart.x(), selectStart.y(), w, h);
                cr->SetPen(curcolp);
                storage.Add(cr);
            }
            break;
        }
        case F_TRI: {
            CTriangle *t = new CTriangle(selectStart, selectEnd);
            t->SetPen(curcolp);
            storage.Add(t);
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
        storage.Del();
        update();
    }

    if (event->key() == Qt::Key_G){
        storage.Grouping();
        update();
    }

    if (event->key() == Qt::Key_U){
        storage.UnGrouping();
        update();
    }
    if (event->key() == Qt::Key_A) {
        auto sel = storage.GetSelectedAll();
        if (sel.size() == 2) {
            storage.AddArrow(sel[0], sel[1]);
            update();
        }
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
    update();
}


bool MainWindow::isClickOnUi(QPoint p) const
{
    QWidget* w = childAt(p);
    if (!w) return false;

    if (w == ui->treeWidget) return true;

    if (ui->treeWidget->isAncestorOf(w)) return true;

    if (qobject_cast<QAbstractButton*>(w)) return true;
    if (qobject_cast<QSpinBox*>(w) || qobject_cast<QAbstractSpinBox*>(w)) return true;

    return false;
}


void MainWindow::on_pushButton_gr_clicked()
{
    storage.Grouping();
    update();
}

void MainWindow::on_pushButton_un_gr_clicked()
{
    storage.UnGrouping();
    update();
}



void MainWindow::on_pushButton_clicked()
{
    auto sel = storage.GetSelectedAll();
    if (sel.size() == 2) {
        storage.AddArrow(sel[0], sel[1]);
        update();
    }
}

