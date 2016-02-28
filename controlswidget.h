#ifndef CONTROLSWIDGET
#define CONTROLSWIDGET

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class ControlsWidget : public QWidget {
    Q_OBJECT

public:
    enum Value {X, Y, R};
    ControlsWidget(Value v, QWidget *parent = 0);
    ~ControlsWidget();

signals:
    void valueChanged(int value);
//public slots:
//    void change(int value);

private:
    QSlider *slider;
    QSpinBox *spinner;
    QLabel *label;
    Value value;
};

#endif // CONTROLSWIDGET

