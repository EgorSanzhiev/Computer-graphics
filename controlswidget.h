#ifndef CONTROLSWIDGET
#define CONTROLSWIDGET

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class ControlsWidget : public QWidget {
    Q_OBJECT

public:
    ControlsWidget(QString title, int minRange, int maxRange, int initValue, QWidget *parent = 0);
    ~ControlsWidget();
    void setValue(int v);

signals:
    void valueChanged(int v);

private:
    QSlider *slider;
    QSpinBox *spinner;
    QLabel *label;
};

#endif // CONTROLSWIDGET

