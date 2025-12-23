#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MyStorage.h>
#include <Figures.h>
#include <Group.h>
#include <QPainter>
#include <QKeyEvent>

#include <Ellipse.h>
#include <Rectangle.h>
#include <Triangle.h>
#include <ConcrFactory.h>
#include <TreeView.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void on_pb_ell_clicked();
    void on_pb_rect_clicked();
    void on_pb_tri_clicked();

    void on_pushButton_color_clicked();

    void on_spinBox_editingFinished();
    void onTreeObjSel(CFigure* obj);

    void on_pushButton_load_clicked();

    void on_pushButton_save_clicked();
    bool isClickOnUi(QPoint p) const;

    void on_pushButton_gr_clicked();
    void on_pushButton_un_gr_clicked();

    void on_pushButton_clicked();
    //void onObjsSelected(const std::vector<CFigure*>& objs);

private:
    Ui::MainWindow *ui;

    MyStorage storage;

    bool isSelecting = false;

    QPoint selectStart;
    QPoint selectEnd;
    QPoint d1;
    QPoint d2;


    enum figure_type{
        F_NONE,
        F_EL,
        F_RECT,
        F_TRI,
    };

    figure_type cur_figure;

    bool isDragging = false;
    QPoint lastpos;

    QColor curcolp = Qt::white;

    ConcrFactory factory;
};
#endif // MAINWINDOW_H
