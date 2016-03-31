#include "lemniscate.h"
#include <QImage>
#include <limits>
#include <iostream>
#include "intmath.h"

using namespace IntMath;

Lemniscate::Lemniscate(int focus1X, int focus1Y, int focus2X, int focus2Y) {
    focus1.x = focus1X;
    focus1.y = focus1Y;
    focus2.x = focus2X;
    focus2.y = focus2Y;
}

void Lemniscate::setX1(int x1) {
    focus1.x = x1;
}

void Lemniscate::setY1(int y1) {
    focus1.y = y1;
}

void Lemniscate::setX2(int x2) {
    focus2.x = x2;
}

void Lemniscate::setY2(int y2) {
    focus2.y = y2;
}

void Lemniscate::read(const QJsonObject &settings) {

}

QJsonObject Lemniscate::serialize() {

}

Point Lemniscate::binarSearch(Point &left, Point &right) {
    Point middle;

    long sqrFocusDistance = power(focus1.sqrDistanceTo(focus2), 2) >> 4;

    middle.x = (left.x + right.x) >> 1;
    middle.y = (left.y + right.y) >> 1;

    long sqrDiff = middle.sqrDistanceTo(focus1) * middle.sqrDistanceTo(focus2) - sqrFocusDistance;

    if (middle.sqrDistanceTo(left) <= 1 || middle.sqrDistanceTo(right) <= 1) {
        return middle;
    }

    if (sqrDiff > 0) {
        return binarSearch(middle, right);
    }

    if (sqrDiff < 0) {
        return binarSearch(left, middle);
    }
}

Point Lemniscate::findNextPoint(Point &currentPoint, Point &previousPoint, int direction) {
    Point nextPoint;
    Point steps[4] = {Point(0, -1), Point(0, 1), Point(-1, 0), Point(1, 0)};
    Point selfCross((focus1.x + focus2.x) >> 1, (focus1.y + focus2.y) >> 1);

    long min = INT_MAX;
    long sqrFocusDistance = power(focus1.sqrDistanceTo(focus2) >> 2, 2);

    for (int i = 0; i < 4; ++i) {
        Point tempPoint = currentPoint + steps[i];

        long z = (selfCross.x - tempPoint.x) * (focus2.y - focus1.y)
                -(selfCross.y - tempPoint.y) * (focus2.x - focus1.x);

        if (direction * z < 0) {
            continue;
        }

        if (tempPoint == previousPoint)
            continue;

        long sqrDiff = IntMath::abs(tempPoint.sqrDistanceTo(focus1) * tempPoint.sqrDistanceTo(focus2) - sqrFocusDistance);

        if (sqrDiff < min) {
            min = sqrDiff;
            nextPoint = tempPoint;
        }
    }

    previousPoint = currentPoint;

    return nextPoint;
}

void Lemniscate::drawHelper(Point &startPoint, Point &endPoint, QImage *backBuffer, int ul) {
    Point selfCross((focus1.x + focus2.x) >> 1, (focus1.y + focus2.y) >> 1);

    int xOffset = backBuffer->width() >> 1;
    int yOffset = backBuffer->height() >> 1;

    backBuffer->setPixel(selfCross.x + xOffset, yOffset - selfCross.y, 0);

    Point currentPoint = startPoint;

    Point previousPoint = startPoint;

    while (currentPoint.sqrDistanceTo(selfCross) > 1) {
        currentPoint = findNextPoint(currentPoint, previousPoint, ul);

        backBuffer->setPixel(currentPoint.x + xOffset, yOffset - currentPoint.y, 0);
    }

    currentPoint = endPoint;

    previousPoint = endPoint;

    while (currentPoint.sqrDistanceTo(selfCross) > 1) {
        currentPoint = findNextPoint(currentPoint, previousPoint, ul);

        backBuffer->setPixel(currentPoint.x + xOffset, yOffset - currentPoint.y, 0);
    }
}

void Lemniscate::draw(QImage *backBuffer) {
    Point startPoint;

    int xOffset = backBuffer->width() >> 1;
    int yOffset = backBuffer->height() >> 1;

    startPoint.x = focus1.x - ((focus2.x - focus1.x) >> 1);
    startPoint.y = focus1.y - ((focus2.y - focus1.y) >> 1);

    startPoint = binarSearch(startPoint, focus1);

    Point endPoint;

    endPoint.x = focus2.x + (focus1.x - startPoint.x);
    endPoint.y = focus2.y + (focus1.y - startPoint.y);

    backBuffer->setPixel(startPoint.x + xOffset, yOffset - startPoint.y, 0);

    backBuffer->setPixel(focus1.x + xOffset, yOffset - focus1.y, 0);

    backBuffer->setPixel(focus2.x + xOffset, yOffset - focus2.y, 0);

    drawHelper(startPoint, endPoint, backBuffer, 1);
    drawHelper(startPoint, endPoint, backBuffer, -1);
}
