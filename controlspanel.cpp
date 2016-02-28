#include <QBoxLayout>
#include "controlspanel.h"

ControlsPanel::ControlsPanel(QWidget *parent) : QGroupBox(parent) {
    xControl = new ControlsWidget(ControlsWidget::Value::X, this);
    yControl = new ControlsWidget(ControlsWidget::Value::Y, this);
    rControl = new ControlsWidget(ControlsWidget::Value::R, this);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(xControl);
    layout->addWidget(yControl);
    layout->addWidget(rControl);

    setLayout(layout);

    controller = Controller::getInstance();

    connect(xControl, SIGNAL(valueChanged(int)), controller, SLOT(setX(int)));
    connect(yControl, SIGNAL(valueChanged(int)), controller, SLOT(setY(int)));
    connect(rControl, SIGNAL(valueChanged(int)), controller, SLOT(setR(int)));
}

ControlsPanel::~ControlsPanel() {
//    delete xControl;
//    delete yControl;
//    delete rControl;
//    delete controller;
}
