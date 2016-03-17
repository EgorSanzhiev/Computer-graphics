#include "drawwidget.h"
#include <QPainter>
#include <QImage>

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    controller = Controller::getInstance();

    connect(controller, SIGNAL(circleUpdated()), this, SLOT(update()));
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

    controller->drawCircle(backBuffer);

    painter.drawImage(0,0, *backBuffer);
}
