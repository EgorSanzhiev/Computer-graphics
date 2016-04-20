#include "figure.h"
#include <math.h>
#include <iostream>
#include <QJsonArray>

Figure::Figure() {
    points = new std::vector<Point>();
}

Figure::~Figure() {
    delete points;
}

Figure::Point::Point(int x, int y, bool onCurve) {
    this->x = x;
    this->y = y;
    this->onCurve = onCurve;
}

Figure::Point Figure::Point::scaleAndShift(double scale, Figure::Point shift)
{
    Point result;

    result.x = round(scale * (double) x) + shift.x;
    result.y = round(scale * (double) y) + shift.y;

    return result;
}

void Figure::read(const QJsonObject &settings) {
    QJsonValue position = settings["position"];
    QJsonValue glyphs = settings["glyphs"];
    QJsonArray figure = glyphs.toObject()["figure"].toArray();
    QJsonValue scale = settings["scale"];
    QJsonValue fill = settings["fill"];

    shift.x = position.toObject()["x"].toInt();
    shift.y = position.toObject()["y"].toInt();

    fillMode = fill.toBool();

    setScale(scale.toInt());

    for (QJsonArray::iterator i = figure.begin(); i != figure.end(); ++i) {
        QJsonObject point = (*i).toObject();

        QJsonObject coords = point["position"].toObject();
        QJsonValue onCurve = point["oncurve"];

        points->push_back(Point(coords["x"].toInt(), coords["y"].toInt(), onCurve.toBool()));
    }

    emit settingsLoaded(shift.x, shift.y, scale.toInt());
}

QJsonObject Figure::serialize() {

}

void Figure::setPixel(QImage *buffer, int x, int y, int color) {
    uchar *backArray = buffer->bits();

    int realX = screenCenter.x + x;
    int realY = screenCenter.y - y;

    if (realX < 0 || realY < 0 || realX > buffer->width() || realY > buffer->height())
        return;

    memset(backArray + realY * buffer->bytesPerLine() + realX * 3 * sizeof(uchar) , color, 3 * sizeof(uchar));
}

double Figure::distance(double x1, double y1, double x2, double y2)
{
    double dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return dist;
}

void Figure::drawBezier(QImage *buffer, Point &start, Point &end, Point &intermediate) {
    double leftX = (double) (start.x + intermediate.x) / 2.0;
    double leftY = (double) (start.y + intermediate.y) / 2.0;
    double rightX = (double) (end.x + intermediate.x) / 2.0;
    double rightY = (double) (end.y + intermediate.y) / 2.0;

    double dist = distance(start.x, start.y, leftX, leftY);

    if (dist < 1.0) {
        drawLine(buffer, start, intermediate);
        drawLine(buffer, intermediate, end);
        return;
    }

    double middleX = (leftX + rightX) / 2.0;
    double middleY = (leftY + rightY) / 2.0;

    Point left(round(leftX), round(leftY));
    Point middle(round(middleX), round(middleY));
    Point right(round(rightX), round(rightY));

    drawBezier(buffer, start, middle, left);
    drawBezier(buffer, middle, end, right);
}

void Figure::putInFillVector(int x, int y, int yMax) {
    int yIndex = (screenCenter.y - y) > 0 ? screenCenter.y - y : 0;
    yIndex = (yIndex < yMax) ? yIndex : (yMax - 1);

    std::vector<int> &borders = fillBorders->at(yIndex);

    auto position = std::find(borders.begin(), borders.end(), x + screenCenter.x);

    if (position == borders.end())
        borders.push_back(x + screenCenter.x);
}

void Figure::setPixel(QImage *buffer, int x, int y, int red, int green, int blue)
{
    uchar *backBuffer = buffer->bits();

    if (x < 0 || y < 0 || x > buffer->width() || y > buffer->height())
        return;

    *(backBuffer + y * buffer->bytesPerLine() + x * 3 * sizeof(uchar)) = red;
    *(backBuffer + y * buffer->bytesPerLine() + (x * 3 + 1) * sizeof(uchar)) = green;
    *(backBuffer + y * buffer->bytesPerLine() + (x * 3 + 2) * sizeof(uchar)) = blue;
}

void Figure::drawLine(QImage *buffer, Point &start, Point &end) {
    int newEndY = std::max(start.y, end.y);
    int newStartY = std::min(start.y, end.y);

    if (fillMode) {
        if (start.x == end.x) {
            for (int y = newStartY; y < newEndY; ++y)
                putInFillVector(start.x, y, buffer->height());

        } else {
            double a = (double) (end.y - start.y) / (end.x - start.x);
            double b = start.y - a * start.x;

            if (a != 0)
                for (int y = newStartY; y < newEndY; ++y) {
                    double x = (y - b) / a;

                    int roundX = round(x);

                    putInFillVector(roundX, y, buffer->height());
                }
        }
    }

    double stepsNum = std::max(abs(start.x - end.x), abs(start.y - end.y));

    double curX = start.x;
    double curY = start.y;

    for (int i = 0; i < stepsNum; ++i) {
        curX += (double) (end.x - start.x) / stepsNum;
        curY += (double) (end.y - start.y) / stepsNum;

        int nextX = round(curX);
        int nextY = round(curY);

        if (outlineMode)
            setPixel(buffer, nextX, nextY, 0);
    }
}

void Figure::draw(QImage *buffer) {
    if (!outlineMode && !fillMode)
        return;

    screenCenter.x = buffer->width() / 2;
    screenCenter.y = buffer->height() / 2;

    fillBorders = new std::vector<std::vector<int>>(buffer->height());

    for (int i = 0; i < points->size(); ++i) {
        int previousIndex = (i - 1) > -1 ? i - 1 : (points->size() - 1);
        int nextIndex = (i + 1) < points->size() ? i + 1 : 0;
        Point currentPoint = points->at(i);
        Point previousPoint = points->at(previousIndex);
        Point nextPoint = points->at(nextIndex);

        if (currentPoint.onCurve && previousPoint.onCurve) {
            Point start = previousPoint.scaleAndShift(scale, shift);
            Point end = currentPoint.scaleAndShift(scale, shift);
            drawLine(buffer, start, end);
        }

        if (!currentPoint.onCurve && previousPoint.onCurve && nextPoint.onCurve) {
            Point start = previousPoint.scaleAndShift(scale, shift);
            Point end = nextPoint.scaleAndShift(scale, shift);
            Point intermediate = currentPoint.scaleAndShift(scale, shift);
            drawBezier(buffer, start, end, intermediate);
        }
    }

    if (fillMode) {
        fill(buffer);
    }
}

void Figure::fill(QImage *buffer) {
    for (int i = 0; i < fillBorders->size(); ++i) {
        std::vector<int> *borders = &(fillBorders->at(i));

        std::sort(borders->begin(), borders->end());

        for (int j = 0; j < borders->size() / 2; ++j) {
            int leftBorder = borders->at(2 * j);
            int rightBorder = borders->at(2 * j + 1);

            if (leftBorder < 0)
                leftBorder = 0;

            if (rightBorder > buffer->width())
                rightBorder = buffer->width();

            if (leftBorder >= buffer->width() || rightBorder <= 0)
                leftBorder = rightBorder;

            for (int x = leftBorder + 1; x < rightBorder; ++x)
                setPixel(buffer, x, i, 0, 0, 255);
        }
    }
}

void Figure::setX(int x) {
    if (x > MAX_SHIFT) {
        x = MAX_SHIFT;
    }

    if (x < MIN_SHIFT) {
        x = MIN_SHIFT;
    }

    shift.x = x;
}

void Figure::setY(int y) {
    if (y > MAX_SHIFT) {
        y = MAX_SHIFT;
    }

    if (y < MIN_SHIFT) {
        y = MIN_SHIFT;
    }

    shift.y = y;
}

int Figure::getX()
{
    return shift.x;
}

int Figure::getY() {
    return shift.y;
}

void Figure::setFillMode(bool fillMode)
{
    this->fillMode = fillMode;
}

void Figure::setOutlineMode(bool outlineMode)
{
    this->outlineMode = outlineMode;
}

void Figure::setScale(int scale) {
    this->scale = scale > 0 ? (1.0 + (double) scale / 100.0) : (1.0 + (double) scale / 1000.0);
}
