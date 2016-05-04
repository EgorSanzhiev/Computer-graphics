#include "controlswidget.h"
#include <QGridLayout>

ControlsWidget::ControlsWidget(QString text, int minRange, int maxRange, QWidget *parent) : QWidget(parent) {
    slider = new QSlider(Qt::Orientation::Horizontal, this);
    spinner = new QSpinBox(this);
    label = new QLabel(this);

    QGridLayout *grid = new QGridLayout;

    grid->addWidget(label, 0, 0, Qt::AlignLeft);
    grid->addWidget(spinner, 0, 1, Qt::AlignRight);
    grid->addWidget(slider, 1, 0, 1, -1);

    setLayout(grid);

    connect(slider, SIGNAL(valueChanged(int)), spinner, SLOT(setValue(int)));

    connect(spinner, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));

    connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));

    label->setText(text);
    slider->setRange(minRange, maxRange);
    spinner->setRange(minRange, maxRange);
}

void ControlsWidget::setValue(int v) {
    slider->setValue(v);
    spinner->setValue(v);
}

ControlsWidget::~ControlsWidget() {
    delete slider;
    delete spinner;
    delete label;
}


