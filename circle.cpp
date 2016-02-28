#include "circle.h"
#include <math.h>

Circle::Circle() {
    this->x = 0;
    this->y = 0;
    this->r = 20;
}

void Circle::draw(QImage *pBackBuffer) {
    if (!pBackBuffer) {
        return;
    }

    uchar *buffer = pBackBuffer->bits();

    int realX = pBackBuffer->width() / 2 + this->x;
    int realY = pBackBuffer->height() / 2 - this->y;

    int upperBorder = realY - this->r;
    int bottomBorder = realY + this->r;

    int windowWidth = pBackBuffer->width();
    int windowHeight = pBackBuffer->height();

    if (upperBorder < 0) {
        upperBorder = 0;
    }

    if (bottomBorder > windowHeight) {
        bottomBorder = windowHeight;
    }

    if (upperBorder >= windowHeight || bottomBorder <= 0) {
        upperBorder = bottomBorder;
    }

    for(int line = upperBorder; line <= bottomBorder; ++line) {
        int cathetus = sqrt(this->r * this->r - (realY - line) * (realY - line));
        int leftBorder = realX - cathetus;
        int rightBorder = realX + cathetus;

        if (leftBorder < 0) {
            leftBorder = 0;
        }

        if (rightBorder > windowWidth) {
            rightBorder = windowWidth;
        }

        if (leftBorder >= windowWidth || rightBorder <= 0) {
            leftBorder = rightBorder;
        }

        int offset = (line * pBackBuffer->bytesPerLine()) + leftBorder * 3 * sizeof(uchar);

        memset(buffer + offset, 0,
               (rightBorder - leftBorder) * 3 * sizeof(uchar));
    }
}

void Circle::setX(int x) {
    this->x = x;
}

void Circle::setY(int y) {
    this->y = y;
}

void Circle::setR(int r) {
    this->r = r;
}
