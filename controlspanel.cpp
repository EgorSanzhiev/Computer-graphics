#include <QBoxLayout>
#include "controlspanel.h"

ControlsPanel::ControlsPanel(QWidget *parent) : QGroupBox(parent) {
    setTitle("Controls panel");

    int minRange = Controller::MIN_RANGE;
    int maxRange = Controller::MAX_RANGE;

    f1XControl = new ControlsWidget("Focus 1 X", minRange, maxRange, Controller::DEFAULT_F1X, this);
    f1YControl = new ControlsWidget("Focus 1 Y", minRange, maxRange, Controller::DEFAULT_F1Y,this);
    f2XControl = new ControlsWidget("Focus 2 X", minRange, maxRange, Controller::DEFAULT_F2X, this);
    f2YControl = new ControlsWidget("Focus 2 Y", minRange, maxRange, Controller::DEFAULT_F2Y, this);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(f1XControl);
    layout->addWidget(f1YControl);
    layout->addWidget(f2XControl);
    layout->addWidget(f2YControl);

    setLayout(layout);

    controller = Controller::getInstance();

    connect(controller, SIGNAL(configLoaded(int,int,int,int)), this, SLOT(changePositions(int,int,int,int)));

    connect(f1XControl, SIGNAL(valueChanged(int)), controller, SLOT(setFocus1X(int)));
    connect(f1YControl, SIGNAL(valueChanged(int)), controller, SLOT(setFocus1Y(int)));
    connect(f2XControl, SIGNAL(valueChanged(int)), controller, SLOT(setFocus2X(int)));
    connect(f2YControl, SIGNAL(valueChanged(int)), controller, SLOT(setFocus2Y(int)));
}

void ControlsPanel::changePositions(int f1X, int f1Y, int f2X, int f2Y) {
    f1XControl->setValue(f1X);
    f1YControl->setValue(f1Y);
    f2XControl->setValue(f2X);
    f2YControl->setValue(f2Y);
}

ControlsPanel::~ControlsPanel() {
//    delete xControl;
//    delete yControl;
//    delete rControl;
}
