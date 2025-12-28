#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <Properties.h>

class PropertiesPanel : public QWidget {
    Q_OBJECT
public:
    explicit PropertiesPanel(QWidget* parent=nullptr);

    void setObject(CProprties* obj);

signals:
    void edited();
    void requestSelectChanged(bool selected);

private slots:
    void onIntChanged(int v);
    void onBoolChanged(int st);
    void onColorClicked();

private:
    void rebuild();
    void clearForm();

    CProprties* object = nullptr;

    QLabel* title = nullptr;
    QFormLayout* form = nullptr;
};
#endif // PROPERTIESPANEL_H
