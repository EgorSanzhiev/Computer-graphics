#include "figure.h"
#include <math.h>
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

void Figure::read(const QJsonObject &settings) {
    QJsonValue position = settings["position"];
    QJsonValue glyphs = settings["glyphs"];
    QJsonArray figure = glyphs.toObject()["figure"].toArray();
    QJsonValue scale = settings["scale"];

    shift.x = position.toObject()["x"].toInt();
    shift.y = position.toObject()["y"].toInt();

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

int Figure::factorial(int n) {
    int fact = 1;

    for (int i = 1; i <= n; ++i) {
        fact *= i;
    }

    return fact;
}

void Figure::setPixel(QImage *buffer, int x, int y, int color) {
    uchar *backArray = buffer->bits();

    int realX = screenCenter.x + x;
    int realY = screenCenter.y - y;

    if (realX < 0 || realY < 0 || realX > buffer->width() || realY > buffer->height())
        return;

    memset(backArray + realY * buffer->bytesPerLine() + realX * 3 * sizeof(uchar) , color, 3 * sizeof(uchar));
}

double Figure::bernstein(int n, int i, double t) {
    double coeff = (double) factorial(n) / (factorial(i) * factorial(n - i));

    double res = coeff * pow(t, i) * pow(1 - t, n - i);

    return res;
}

void Figure::drawBezier(QImage *buffer, Point &start, Point &end, Point &intermediate) {
    double diff = 1.0 / (double) steps;
    int n = 2;

    double startX = scale * start.x + shift.x;
    double endX = scale * end.x + shift.x;
    double startY = scale * start.y + shift.y;
    double endY = scale * end.y + shift.y;
    double intermediateX = scale * intermediate.x + shift.x;
    double intermediateY = scale * intermediate.y + shift.y;

    for (double t = 0.0; t <= 1.0; t += diff) {
        if (t > 1.0)
            t = 1.0;
        int nextX = round(startX * bernstein(n, 0, t) + intermediateX * bernstein(n, 1, t) + endX * bernstein(n, 2, t));

        int nextY = round(startY * bernstein(n, 0, t) + intermediateY * bernstein(n, 1, t) + endY * bernstein(n, 2, t));

        setPixel(buffer, nextX, nextY, 0);
    }
}

void Figure::drawLine(QImage *buffer, Point &start, Point &end) {
    int startX = round(scale * start.x + shift.x);
    int endX = round(scale * end.x + shift.x);
    int startY = round(scale * (start.y < end.y ? start.y : end.y) + shift.y);
    int endY = round(scale * (start.y > end.y ? start.y : end.y) + shift.y);

    if (end.x == start.x) {
        for (int y = startY; y <= endY; ++y) {
            setPixel(buffer, startX, y, 0);
        }

        return;
    }

    double a = (double) (endY - startY) / (endX - startX);
    double b = startY - a * startX;

    for (int x = startX; x <= endX; ++x) {
        double y = a * x + b;

        setPixel(buffer, x, y, 0);
    }
}

void Figure::draw(QImage *buffer) {
    screenCenter.x = buffer->width() / 2;
    screenCenter.y = buffer->height() / 2;

    for (int i = 1; i < points->size(); ++i) {
        int previousIndex = (i - 1) > -1 ? i - 1 : points->size() - 1;
        int nextIndex = (i + 1) < points->size() ? i + 1 : 0;
        Point currentPoint = points->at(i);
        Point previousPoint = points->at(previousIndex);
        Point nextPoint = points->at(nextIndex);

        if (currentPoint.onCurve && previousPoint.onCurve) {
            if (previousPoint.x < currentPoint.x)
                drawLine(buffer, previousPoint, currentPoint);
            else
                drawLine(buffer, currentPoint, previousPoint);
        }

        if (!currentPoint.onCurve && previousPoint.onCurve && nextPoint.onCurve) {
            drawBezier(buffer, previousPoint, nextPoint, currentPoint);
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

void Figure::setScale(int scale) {
    this->scale = scale > 0 ? (1.0 + (double) scale / 100.0) : (1.0 + (double) scale / 1000.0);
}

int Figure::getX() {
    return shift.x;
}

int Figure::getY() {
    return shift.y;
}
