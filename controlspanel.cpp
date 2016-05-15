#include <QBoxLayout>
#include "controlspanel.h"

ControlsPanel::ControlsPanel(QWidget *parent) : QGroupBox(parent) {
    xControl = new ControlsWidget("X", -360, 360, this);
    yControl = new ControlsWidget("Y", -360, 360, this);
    scaleControl = new ControlsWidget("Scale", -1000, 1000, this);
    filteringControl = new QComboBox();

    filteringControl->addItem("nearest");
    filteringControl->addItem("bilinear");

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(xControl);
    layout->addWidget(yControl);
    layout->addWidget(scaleControl);
    layout->addWidget(filteringControl);

    setLayout(layout);

    controller = Controller::getInstance();

    connect(xControl, SIGNAL(valueChanged(int)), controller, SLOT(setX(int)));
    connect(yControl, SIGNAL(valueChanged(int)), controller, SLOT(setY(int)));
    connect(scaleControl, SIGNAL(valueChanged(int)), controller, SLOT(setScale(int)));
    connect(filteringControl, SIGNAL(activated(QString)), controller, SLOT(setFilteringType(QString)));

    connect(controller, SIGNAL(configLoaded(int,int,int)), this, SLOT(changePositions(int,int,int)));
    connect(controller, SIGNAL(modelDragged(int,int)), this, SLOT(changePositions(int,int)));
}

void ControlsPanel::changePositions(int x1, int y1, int scale) {
    xControl->setValue(x1);
    yControl->setValue(y1);
    scaleControl->setValue(scale);
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
