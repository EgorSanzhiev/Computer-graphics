#include <QBoxLayout>
#include "controlspanel.h"

ControlsPanel::ControlsPanel(QWidget *parent) : QGroupBox(parent) {
    x1Control = new ControlsWidget("X1", -500, 500, this);
    y1Control = new ControlsWidget("Y1", -500, 500, this);
    x2Control = new ControlsWidget("X2", -500, 500, this);
    y2Control = new ControlsWidget("Y2", -500, 500, this);

    changePositions(-20, 0, 20, 0);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(x1Control);
    layout->addWidget(y1Control);
    layout->addWidget(x2Control);
    layout->addWidget(y2Control);

    setLayout(layout);

    controller = Controller::getInstance();

    connect(x1Control, SIGNAL(valueChanged(int)), controller, SLOT(setX1(int)));
    connect(y1Control, SIGNAL(valueChanged(int)), controller, SLOT(setY1(int)));
    connect(x2Control, SIGNAL(valueChanged(int)), controller, SLOT(setX2(int)));
    connect(y2Control, SIGNAL(valueChanged(int)), controller, SLOT(setY2(int)));

    connect(controller, SIGNAL(configLoaded(int,int,int, int)), this, SLOT(changePositions(int,int,int, int)));
}

void ControlsPanel::changePositions(int x1, int y1, int x2, int y2) {
    x1Control->setValue(x1);
    y1Control->setValue(y1);
    x2Control->setValue(x2);
    y2Control->setValue(y2);
}

ControlsPanel::~ControlsPanel() {
    delete x1Control;
    delete y1Control;
    delete x2Control;
    delete y2Control;
}
