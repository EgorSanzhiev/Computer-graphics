#include "circle.h"
#include <math.h>

Circle::Circle(int x, int y, int r) {
    this->x = x;
    this->y = y;
    this->r = r;
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

    for(int line = upperBorder; line <= bottomBorder - 1; ++line) {
        double cathetus = sqrt(this->r * this->r - (realY - line) * (realY - line));
        double leftBorder = (double) realX - cathetus;
        double rightBorder = (double) realX + cathetus;

        if (leftBorder < 0) {
            leftBorder = 0;
        }

        if (rightBorder > windowWidth) {
            rightBorder = windowWidth;
        }

        if (leftBorder >= windowWidth || rightBorder <= 0) {
            leftBorder = rightBorder;
        }

        int offset = (line * pBackBuffer->bytesPerLine()) + (int) leftBorder * 3 * sizeof(uchar);

        memset(buffer + offset, 0,
               (int) (rightBorder - leftBorder) * 3 * sizeof(uchar));
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

