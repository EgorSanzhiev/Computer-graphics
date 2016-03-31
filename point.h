#ifndef POINT_H
#define POINT_H


struct Point
{
    int x;
    int y;

    Point();
    Point(int x, int y);
    long sqrDistanceTo(Point &p);
    Point operator =(const Point &p);
    Point operator +(const Point &p);
    friend bool operator ==(const Point &p1, const Point &p2);
    friend bool operator !=(const Point &p1, const Point &p2);
};

bool operator !=(const Point &p1, const Point &p2);
bool operator ==(const Point &p1, const Point &p2);

#endif // POINT_H
