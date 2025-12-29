#include <PropertiesPanel.h>
#include <QVBoxLayout>
#include <QColorDialog>

PropertiesPanel::PropertiesPanel(QWidget* parent) : QWidget(parent)
{
    auto* v = new QVBoxLayout(this);
    title = new QLabel("");
    title->setAlignment(Qt::AlignCenter);
    v->addWidget(title);

    form = new QFormLayout();
    v->addLayout(form);
}

void PropertiesPanel::setObject(CProprties* obj)
{
    object = obj;
    rebuild();
}

void PropertiesPanel::clearForm()
{
    while (form->rowCount() > 0) form->removeRow(0);
}

void PropertiesPanel::rebuild()
{
    clearForm();

    if (!object) {
        title->clear();
        title->setVisible(false);
        return;
    }

    title->setVisible(true);
    title->setText("Properties");

    int n = object->propCount();
    for (int i = 0; i < n; ++i) {
        PropMeta m = object->propMeta(i);
        QVariant val = object->getProp(i);

        QWidget* editor = nullptr;

        if (m.type == PropMeta::Type::Bool) {
            auto* cb = new QCheckBox();
            cb->setChecked(val.toBool());
            cb->setEnabled(m.editable);
            cb->setProperty("propIndex", i);
            connect(cb, &QCheckBox::stateChanged, this, &PropertiesPanel::onBoolChanged);
            editor = cb;
        }
        else if (m.type == PropMeta::Type::Int) {
            auto* sb = new QSpinBox();
            sb->setRange(m.minInt, m.maxInt);
            sb->setValue(val.toInt());
            sb->setEnabled(m.editable);
            sb->setProperty("propIndex", i);
            connect(sb, QOverload<int>::of(&QSpinBox::valueChanged), this, &PropertiesPanel::onIntChanged);
            editor = sb;
        }
        else if (m.type == PropMeta::Type::Color) {
            auto* btn = new QPushButton();
            btn->setEnabled(m.editable);
            btn->setProperty("propIndex", i);
            //QColor c = val.value<QColor>();
            connect(btn, &QPushButton::clicked, this, &PropertiesPanel::onColorClicked);
            editor = btn;
        }

        if (editor)
            form->addRow(m.name, editor);
    }
}

void PropertiesPanel::onIntChanged(int v)
{
    if (!object) return;
    QObject* s = sender();
    int idx = s->property("propIndex").toInt();
    object->setProp(idx, QVariant(v));
    emit edited();
}

void PropertiesPanel::onBoolChanged(int st)
{/*
    if (!object) return;
    QObject* s = sender();
    int idx = s->property("propIndex").toInt();
    object->setProp(idx, QVariant(st == Qt::Checked));
    emit edited();*/
    if (!object) return;

    QObject* s = sender();
    int idx = s->property("propIndex").toInt();

    bool b = (st == Qt::Checked);

    if (idx == 0) {
        emit requestSelectChanged(b);
        return;
    }

    object->setProp(idx, QVariant(b));
    emit edited();
}

void PropertiesPanel::onColorClicked()
{
    if (!object) return;
    QObject* s = sender();
    int idx = s->property("propIndex").toInt();

    QColor cur = object->getProp(idx).value<QColor>();
    QColor c = QColorDialog::getColor(cur, this, "Цвет");
    if (!c.isValid()) return;

    object->setProp(idx, QVariant(c));

    emit edited();
}


