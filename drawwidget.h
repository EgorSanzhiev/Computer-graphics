#ifndef DRAWWIDGET
#define DRAWWIDGET

#include <QWidget>
#include "controller.h"

class DrawWidget : public QWidget {
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Controller *controller;
    QImage *backBuffer;
    QPoint startDragPoint;
};

#endif // DRAWWIDGET

