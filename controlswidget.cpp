#include "controlswidget.h"
#include <QGridLayout>

ControlsWidget::ControlsWidget(Type v, QWidget *parent) : QWidget(parent) {
    type = v;

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

    switch (type) {
    case X:
        label->setText("Position X");
        slider->setRange(-500, 500);
        spinner->setRange(-500, 500);
        break;
    case Y:
        label->setText("Position Y");
        slider->setRange(-500, 500);
        spinner->setRange(-500, 500);
        break;
    case R:
        label->setText("R");
        slider->setRange(0, 500);
        spinner->setRange(0, 500);
        slider->setSliderPosition(20);
    }
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


