#include "drawwidget.h"
#include <QPainter>
#include <QImage>
#include <QMouseEvent>

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    controller = Controller::getInstance();

    connect(controller, SIGNAL(modelUpdated()), this, SLOT(update()));
}

DrawWidget::~DrawWidget()
{
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    backBuffer = new QImage(width(), height(), QImage::Format_RGB888);

    uchar* pubBuffer = backBuffer->bits();

    if (!pubBuffer) {
        return;
    }

    memset(pubBuffer, 255, backBuffer->byteCount());

    controller->draw(backBuffer);

    painter.drawImage(0,0, *backBuffer);
}

void DrawWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startDragPoint = event->pos();
        controller->startDrag();
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint shift = event->pos() - startDragPoint;

        controller->drag(shift.x(), -shift.y());
    }
}
