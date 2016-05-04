#ifndef CONTROLSWIDGET
#define CONTROLSWIDGET

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class ControlsWidget : public QWidget {
    Q_OBJECT

public:
    ControlsWidget(QString text, int minRange, int maxRange, QWidget *parent = 0);
    ~ControlsWidget();
    void setValue(int v);

signals:
    void valueChanged(int val);

private:
    QSlider *slider;
    QSpinBox *spinner;
    QLabel *label;
};

#endif // CONTROLSWIDGET

