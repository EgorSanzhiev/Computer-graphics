#include <QBoxLayout>
#include "controlspanel.h"

ControlsPanel::ControlsPanel(QWidget *parent) : QGroupBox(parent) {
    xControl = new ControlsWidget(ControlsWidget::Type::X, this);
    yControl = new ControlsWidget(ControlsWidget::Type::Y, this);
    rControl = new ControlsWidget(ControlsWidget::Type::R, this);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(xControl);
    layout->addWidget(yControl);
    layout->addWidget(rControl);

    setLayout(layout);

    controller = Controller::getInstance();

    connect(xControl, SIGNAL(valueChanged(int)), controller, SLOT(setX(int)));
    connect(yControl, SIGNAL(valueChanged(int)), controller, SLOT(setY(int)));
    connect(rControl, SIGNAL(valueChanged(int)), controller, SLOT(setR(int)));

    connect(controller, SIGNAL(configLoaded(int,int,int)), this, SLOT(changePositions(int,int,int)));
}

void ControlsPanel::changePositions(int x, int y, int r) {
    xControl->setValue(x);
    yControl->setValue(y);
    rControl->setValue(r);
}

ControlsPanel::~ControlsPanel() {
    delete xControl;
    delete yControl;
    delete rControl;
    delete controller;
}
