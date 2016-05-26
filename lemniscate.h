#ifndef LEMNISCATE_H
#define LEMNISCATE_H

#include <QObject>
#include "iserializable.h"

class Lemniscate : public QObject
{
    Q_OBJECT

private:
    struct Point {
        int x;
        int y;

        Point()
        {}

        Point(int x, int y)
            : x(x),
              y(y)
        {}

        Point operator +(const Point &p);
        int sqrDistanceTo(const Point &p);
        bool operator ==(const Point &p);
    };

    struct Vector {
        int x;
        int y;

        Vector(const Point &start, const Point &end)
            : x(end.x - start.x),
              y(end.y - start.y)
        {}

        Vector(int x, int y)
            : x(x),
              y(y)
        {}

        Vector operator -();
        Vector operator *(const int constant);
        int operator *(const Vector &v);
    };

    Point focus1;
    Point focus2;

    void setPixel(const Point &p, QImage *buffer);
    void drawPart(Point &start, Point &end, Vector &focusVector, int direction, QImage *buffer);
    Point findStartPoint(const Point &left, const Point &right);
    Point findEndPoint(const Point &left, const Point &right);

public:
    explicit Lemniscate(int focus1X = -20, int focus1Y = 16, int focus2X = 20, int focus2Y = 0)
        : focus1(focus1X, focus1Y),
          focus2(focus2X, focus2Y)
    {}

    void draw(QImage *buffer);

signals:
    void settingsLoaded(int f1X, int f1Y, int f2X, int f2Y);

public slots:
    void setFocus1X(int x);
    void setFocus1Y(int y);
    void setFocus2X(int x);
    void setFocus2Y(int y);
};

#endif // LEMNISCATE_H
