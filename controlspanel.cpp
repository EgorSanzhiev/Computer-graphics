#include <QBoxLayout>
#include "controlspanel.h"

ControlsPanel::ControlsPanel(QWidget *parent) : QGroupBox(parent) {
    xControl = new ControlsWidget("X", -1000, 1000, this);
    yControl = new ControlsWidget("Y", -1000, 1000, this);
    scaleControl = new ControlsWidget("Scale", -1000, 1000, this);

    fillControl = new QCheckBox("Fill");
    outLineControl = new QCheckBox("Outline");

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(xControl);
    layout->addWidget(yControl);
    layout->addWidget(scaleControl);
    layout->addWidget(fillControl);
    layout->addWidget(outLineControl);

    setLayout(layout);

    controller = Controller::getInstance();

    connect(xControl, SIGNAL(valueChanged(int)), controller, SLOT(setX(int)));
    connect(yControl, SIGNAL(valueChanged(int)), controller, SLOT(setY(int)));
    connect(scaleControl, SIGNAL(valueChanged(int)), controller, SLOT(setScale(int)));

    connect(controller, SIGNAL(configLoaded(int,int,int, bool, bool)), this, SLOT(changePositions(int,int,int, bool, bool)));
    connect(controller, SIGNAL(modelDragged(int,int)), this, SLOT(changePositions(int,int)));
}

void ControlsPanel::changePositions(int x1, int y1, int scale, bool fillMode, bool outlineMode) {
    xControl->setValue(x1);
    yControl->setValue(y1);
    scaleControl->setValue(scale);
    fillControl->setChecked(fillMode);
    outLineControl->setChecked(outlineMode);
}

void ControlsPanel::changePositions(int x, int y) {
    xControl->setValue(x);
    yControl->setValue(y);
}

ControlsPanel::~ControlsPanel() {
    delete xControl;
    delete yControl;
    delete scaleControl;
}
