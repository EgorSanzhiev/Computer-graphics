#include "drawwidget.h"
#include <QPainter>
#include <QImage>

DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{
    this->circle = new Circle(this);
}

DrawWidget::~DrawWidget()
{
    delete this->circle;
}

void DrawWidget::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    QImage backBuffer(width(), height(), QImage::Format_RGB888);

    uchar* pubBuffer = backBuffer.bits();
    if (!pubBuffer)
    {
        return;
    }
    memset(pubBuffer, 255, backBuffer.byteCount());
    //
    if (this->circle)
    {
        this->circle->draw(&backBuffer);
    }
    //
    painter.drawImage(0,0, backBuffer);
}
