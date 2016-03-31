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

private slots:
    void changePositions(int x, int y, int x2, int y2);

private:
    ControlsWidget *x1Control;
    ControlsWidget *y1Control;
    ControlsWidget *x2Control;
    ControlsWidget *y2Control;
    Controller *controller;
};

#endif // CONTROLSPANEL

