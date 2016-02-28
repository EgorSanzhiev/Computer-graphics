#ifndef DRAWWIDGET
#define DRAWWIDGET

#include <QWidget>
#include "circle.h"
#include "controller.h"

class DrawWidget : public QWidget {
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    Controller *controller;

};

#endif // DRAWWIDGET

