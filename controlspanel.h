#ifndef CONTROLSPANEL
#define CONTROLSPANEL

#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
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
    void scaleChanged(int r);

private slots:
    void changePositions(int x, int y, int scale);
    void changePositions(int x, int y);

private:
    ControlsWidget *xControl;
    ControlsWidget *yControl;
    ControlsWidget *scaleControl;
    QCheckBox *fillControl;
    QCheckBox *outLineControl;
    Controller *controller;
};

#endif // CONTROLSPANEL

