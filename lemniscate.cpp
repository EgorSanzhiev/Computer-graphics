#include "lemniscate.h"
#include <QImage>
#include "intmath.h"

using namespace IntMath;

Lemniscate::Point Lemniscate::Point::operator +(const Point &p)
{
    return Point(this->x + p.x, this->y + p.y);
}

int Lemniscate::Point::sqrDistanceTo(const Lemniscate::Point &p)
{
    return power(this->x - p.x, 2) + power(this->y - p.y, 2);
}

bool Lemniscate::Point::operator ==(const Lemniscate::Point &p)
{
    return (this->x == p.x) && (this->y == p.y);
}

Lemniscate::Vector Lemniscate::Vector::operator -()
{
    return Vector(-x, -y);
}

Lemniscate::Vector Lemniscate::Vector::operator *(const int constant) {
    return Vector(constant * this->x, constant * this->y);
}

int Lemniscate::Vector::operator *(const Lemniscate::Vector &v)
{
    return this->x * v.y - this->y * v.x;
}

void Lemniscate::setPixel(const Point &p, QImage *buffer)
{
    uint windowWidth = buffer->width();
    uint windowHeight = buffer->height();

    int realX = (windowWidth >> 1) + p.x;
    int realY = (windowHeight >> 1) - p.y;

    uchar *byteBuffer = buffer->bits();

    if (realX < 0 || realX > (int) windowWidth || realY < 0 || realY > (int) windowHeight)
        return;

    int bytesPerLine = buffer->bytesPerLine();

    memset(byteBuffer + realY * bytesPerLine + realX * 3 *  sizeof(uchar),
           0, 3 * sizeof(uchar));
}

Lemniscate::Point Lemniscate::findStartPoint(const Point &left, const Point &right)
{
    Point middle((left.x + right.x) >> 1, (left.y + right.y) >> 1);

    int focusDistance = power(focus1.sqrDistanceTo(focus2) >> 2, 2);

    int checkInvariant = middle.sqrDistanceTo(focus1) * middle.sqrDistanceTo(focus2);

    int dbginf = checkInvariant - focusDistance;

    if (middle.sqrDistanceTo(right) < 2 || middle.sqrDistanceTo(left) < 2)
        return middle;

    if (checkInvariant < focusDistance)
        return findStartPoint(left, middle);
    else
        return findStartPoint(middle, right);
}

Lemniscate::Point Lemniscate::findEndPoint(const Lemniscate::Point &left, const Lemniscate::Point &right)
{
    Point middle((left.x + right.x) >> 1, (left.y + right.y) >> 1);

    int focusDistance = power(focus1.sqrDistanceTo(focus2) >> 2, 2);

    int checkInvariant = middle.sqrDistanceTo(focus1) * middle.sqrDistanceTo(focus2);

    int dbginf = checkInvariant - focusDistance;

    if (middle.sqrDistanceTo(right) < 2 || middle.sqrDistanceTo(left) < 2)
        return middle;

    if (checkInvariant > focusDistance)
        return findEndPoint(left, middle);
    else
        return findEndPoint(middle, right);
}

void Lemniscate::drawPart(Point &start, Point &end, Vector &focusVector, int direction, QImage *buffer)
{
    Point nbrs[4] = {Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0)};

    Point currentPoint = start;
    Point previousPoint = start;

    int focusDistance = power(focus1.sqrDistanceTo(focus2) >> 2, 2);

    while (currentPoint.sqrDistanceTo(end) > 1) {
        int min = INT_MAX;
        Point nextPoint;
        for (int i = 0; i < 4; ++i) {
            Point nextNbr = currentPoint + nbrs[i];

            Vector v(nextNbr, end);

            int vectMltp = v * focusVector;

            if (vectMltp * direction < 0 || nextNbr == previousPoint)
                continue;

            int currentDistance = nextNbr.sqrDistanceTo(focus1) * nextNbr.sqrDistanceTo(focus2);

            int diff = IntMath::abs(currentDistance - focusDistance);

            if (diff < min) {
                nextPoint = nextNbr;

                min = diff;
            }
        }

        setPixel(nextPoint, buffer);
        previousPoint = currentPoint;
        currentPoint = nextPoint;
    }
}


void Lemniscate::setFocus1X(int x) {
    focus1.x = x;
}

void Lemniscate::setFocus1Y(int y) {
    focus1.y = y;
}

void Lemniscate::setFocus2X(int x) {
    focus2.x = x;
}

void Lemniscate::setFocus2Y(int y) {
    focus2.y = y;
}

void Lemniscate::draw(QImage *buffer) {
    Vector focusVector(focus2, focus1);

    setPixel(focus1, buffer);
    setPixel(focus2, buffer);

    Vector v = focusVector * 2;

    Point startPoint(v.x + focus2.x, v.y + focus2.y);
    Point endPoint((-v).x + focus1.x, (-v).y + focus1.y);

    startPoint = findStartPoint(startPoint, focus1);
    endPoint = findEndPoint(focus2, endPoint);

    setPixel(startPoint, buffer);
    setPixel(endPoint, buffer);

    focusVector = -focusVector;

    Point selfCross((focus1.x + focus2.x) >> 1, (focus1.y + focus2.y) >> 1);

    drawPart(startPoint, selfCross, focusVector, 1, buffer);
    drawPart(endPoint, selfCross, focusVector, 1, buffer);
    drawPart(startPoint, selfCross, focusVector, -1, buffer);
    drawPart(endPoint, selfCross, focusVector, -1, buffer);
}

