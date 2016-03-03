#ifndef CONTROLSWIDGET
#define CONTROLSWIDGET

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class ControlsWidget : public QWidget {
    Q_OBJECT

public:
    enum Type {X, Y, R};
    ControlsWidget(Type v, QWidget *parent = 0);
    ~ControlsWidget();
    void setValue(int v);

signals:
    void valueChanged(int type);

private:
    QSlider *slider;
    QSpinBox *spinner;
    QLabel *label;
    Type type;
};

#endif // CONTROLSWIDGET

