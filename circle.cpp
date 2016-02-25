#include "circle.h"
#include <math.h>

Circle::Circle(QObject *parent): QObject(parent) {
    this->x = 0;
    this->y = 0;
    this->r = 200;
}

void Circle::draw(QImage *pBackBuffer) {
    if (!pBackBuffer) {
        return;
    }

    uchar *buffer = pBackBuffer->bits();

    int realX = this->x + pBackBuffer->width() / 2;
    int realY = this->y + pBackBuffer->height() / 2;

    int upperBorder = realY - this->r;
    int bottomBorder = realY + this->r;

    for(int line = upperBorder; line <= bottomBorder; ++line) {
        int cathetus = sqrt(this->r * this->r - (realY - line) * (realY - line));
        int leftBorder = realX - cathetus;
        int rightBorder = realX + cathetus;

        int db;

        if (line == realY) {
            db = 0;
        }

        db = pBackBuffer->bytesPerLine();

        int offset = (line * pBackBuffer->bytesPerLine()) + leftBorder * 3 * sizeof(uchar);

        memset(buffer + offset, 0,
               (rightBorder - leftBorder) * 3 * sizeof(uchar));


    }
}
