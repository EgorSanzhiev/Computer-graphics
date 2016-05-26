#ifndef CONTROLSPANEL
#define CONTROLSPANEL

#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include "controlswidget.h"
#include "controller.h"

class ControlsPanel : public QGroupBox {
    Q_OBJECT
public:
    ControlsPanel(QWidget *parent = 0);
    ~ControlsPanel();
signals:


private slots:
    void changePositions(int f1X, int f1Y, int f2X, int f2Y);

private:
    ControlsWidget *f1XControl;
    ControlsWidget *f1YControl;
    ControlsWidget *f2XControl;
    ControlsWidget *f2YControl;
    Controller *controller;
};

#endif // CONTROLSPANEL

