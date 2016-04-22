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
    Point result = *this;

    result.x = round(scale * (double) x) + shift.x;
    result.y = round(scale * (double) y) + shift.y;

    return result;
}

Figure::Point Figure::Point::toRealCoordinates(Figure::Point center, int maxX, int maxY)
{
    Point result = *this;

//    result.x = std::max(0, center.x + this->x);
//    result.x = std::min(maxX, result.x);

    result.x = center.x + this->x;

//    result.y = std::max(0, center.y - this->y);
//    result.y = std::min(maxY, result.y);

    result.y = center.y - this->y;

    return result;
}

Figure::Point &Figure::Point::operator=(const Figure::Point &p)
{
    this->x = p.x;
    this->y = p.y;
    this->onCurve = p.onCurve;

    return *this;
}

void Figure::read(const QJsonObject &settings) {
    QJsonValue position = settings["position"];
    QJsonValue glyphs = settings["glyphs"];
    QJsonArray figure = glyphs.toObject()["figure"].toArray();
    QJsonValue scale = settings["scale"];
    QJsonValue fill = settings["fill"];
    QJsonValue outline = settings["outline"];

    shift.x = position.toObject()["x"].toInt();
    shift.y = position.toObject()["y"].toInt();

    fillMode = fill.toBool();
    outlineMode = outline.toBool();

    setScale(scale.toInt());

    for (QJsonArray::iterator i = figure.begin(); i != figure.end(); ++i) {
        QJsonObject point = (*i).toObject();

        QJsonObject coords = point["position"].toObject();
        QJsonValue onCurve = point["oncurve"];

        points->push_back(Point(coords["x"].toInt(), coords["y"].toInt(), onCurve.toBool()));
    }

    emit settingsLoaded(shift.x, shift.y, scale.toInt(), fillMode, outlineMode);
}

QJsonObject Figure::serialize() {
    QJsonObject result;

    QJsonObject position;
    QJsonValue positionX(shift.x);
    QJsonValue positionY(shift.y);
    QJsonValue scaleValue(integerScale);
    QJsonValue outlineValue(outlineMode);
    QJsonValue fillValue(fillMode);
    QJsonObject glyphs;
    QJsonArray figureArray;

    position.insert("x", positionX);
    position.insert("y", positionY);

    QJsonValue positionValue(position);

    for (Point p: *points) {
        QJsonObject point;
        QJsonObject coords;
        QJsonValue x(p.x), y(p.y), onCurve(p.onCurve);

        coords.insert("x", x);
        coords.insert("y", y);

        QJsonValue coordsValue(coords);

        point.insert("position", coordsValue);
        point.insert("oncurve", onCurve);

        QJsonValue pointValue(point);

        figureArray.push_back(pointValue);
    }

    glyphs.insert("figure", figureArray);

    result.insert("position", positionValue);
    result.insert("scale", scaleValue);
    result.insert("outline", outlineValue);
    result.insert("fill", fillValue);
    result.insert("glyphs", glyphs);

    return result;
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

    double middleX = (leftX + rightX) / 2.0;
    double middleY = (leftY + rightY) / 2.0;

    double a1 = (double) (end.y - start.y) / (end.x - start.x);
    double b1 = end.y - a1 * end.x;

    double a2 = -1 / a1;
    double b2 = middleY - a2 * middleX;

    double crossX = (b2 - b1) / (a1 - a2);
    double crossY = a1 * crossX + b1;

    if (a1 == 0) {
        crossX = middleX;
        crossY = start.y;
    }

    if (end.x == start.x) {
        crossX = start.x;
        crossY = middleY;
    }

    double dist = distance(crossX, crossY, middleX, middleY);

    if (dist < 1.0) {
        drawLine(buffer, start, intermediate);
        drawLine(buffer, intermediate, end);
        return;
    }

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
    int height = buffer->height();
    int width = buffer->width();
    int pointsNum = points->size();

    for (int y = 0; y < height; ++y) {
        std::vector<int> borders;

        for (int i = 0; i < pointsNum; ++i) {
            int previousIndex = (i - 1) > -1 ? i - 1 : (points->size() - 1);
            int nextIndex = (i + 1) < points->size() ? i + 1 : 0;

            Point currentPoint = points->at(i).scaleAndShift(scale, shift);
            Point previousPoint = points->at(previousIndex).scaleAndShift(scale, shift);
            Point nextPoint = (points->at(nextIndex)).scaleAndShift(scale, shift);

            currentPoint = currentPoint.toRealCoordinates(screenCenter, width, height);
            previousPoint = previousPoint.toRealCoordinates(screenCenter, width, height);
            nextPoint = nextPoint.toRealCoordinates(screenCenter, width, height);

            if (currentPoint.onCurve && previousPoint.onCurve) {
                int maxY = std::max(currentPoint.y, previousPoint.y);
                int minY = std::min(currentPoint.y, previousPoint.y);

                if (maxY >= y && minY < y)
                    if (currentPoint.x == previousPoint.x) {
                            borders.push_back(currentPoint.x);

                    } else {
                        double a = (double) (currentPoint.y - previousPoint.y) / (currentPoint.x - previousPoint.x);
                        double b = currentPoint.y - a * currentPoint.x;

                        if (a != 0) {
                            double x = ((double) y - b) / a;

                            int roundX = round(x);

                            borders.push_back(roundX);
                        }
                    }
            }

            if (!currentPoint.onCurve && previousPoint.onCurve && nextPoint.onCurve) {
                int maxY = std::max(previousPoint.y, std::max(currentPoint.y, nextPoint.y));
                int minY = std::min(previousPoint.y, std::min(currentPoint.y, nextPoint.y));

                if (maxY < y || minY >= y)
                    continue;

                bool oneRoot = false;

                double denominator = previousPoint.y - 2 * currentPoint.y + nextPoint.y;
                double discriminant = denominator * y + currentPoint.y * currentPoint.y - previousPoint.y * nextPoint.y;
                if (discriminant < 0)
                    continue;

                double numerator1 = previousPoint.y - currentPoint.y + sqrt(discriminant);
                double numerator2 = previousPoint.y - currentPoint.y - sqrt(discriminant);

                if (denominator == 0) {
                    if (previousPoint.y != currentPoint.y) {
                        denominator = 2 * (currentPoint.y - previousPoint.y);
                        numerator1 = y - previousPoint.y;
                    }
                    else {
                        denominator = sqrt(nextPoint.y - currentPoint.y);
                        numerator1 = sqrt(y - previousPoint.y);
                    }

                    oneRoot = true;
                }

                double t1 = numerator1 / denominator;
                double t2 = numerator2 / denominator;

                double x1 = (1 - t1) * (1 - t1) * previousPoint.x + 2 * t1 * (1 - t1) * currentPoint.x + t1 * t1 * nextPoint.x;
                double x2 = (1 - t2) * (1 - t2) * previousPoint.x + 2 * t2 * (1 - t2) * currentPoint.x + t2 * t2 * nextPoint.x;

                if (t1 <= 1.0 && t1 >= 0.0)
                    borders.push_back(round(x1));

                if (!oneRoot && t2 <= 1.0 && t2 >= 0.0)
                    borders.push_back(round(x2));

            }
        }


        std::sort(borders.begin(), borders.end());

        int size = borders.size();

        for (int j = 0; j < size / 2; ++j) {
            int leftBorder = borders.at(2 * j);
            int rightBorder = borders.at(2 * j + 1);

            if (leftBorder < 0)
                leftBorder = 0;

            if (rightBorder > buffer->width())
                rightBorder = buffer->width();

            if (leftBorder >= buffer->width() || rightBorder <= 0)
                leftBorder = rightBorder;

            for (int x = leftBorder + 1; x < rightBorder; ++x)
                setPixel(buffer, x, y, 0, 0, 255);
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
    integerScale = scale;
    this->scale = scale > 0 ? (1.0 + (double) scale / 100.0) : (1.0 + (double) scale / 1000.0);
}
