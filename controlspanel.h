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
    void xChanged(int x);
    void yChanged(int y);
    void rChanged(int r);

private:
    ControlsWidget *xControl;
    ControlsWidget *yControl;
    ControlsWidget *rControl;
    Controller *controller;
};

#endif // CONTROLSPANEL

