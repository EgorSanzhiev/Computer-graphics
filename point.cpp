#include "point.h"
#include "intmath.h"

using namespace IntMath;

long IntMath::power(long a, long b) {
    long result = 1;
    for (long i = 0; i < b; ++i) {
        result *= a;
    }

    return result;
}

long IntMath::abs(long a) {
    return a >= 0 ? a : -a;
}

Point::Point() {

}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

long Point::sqrDistanceTo(Point &p) {
    return (power(this->x - p.x, 2) + power(this->y - p.y, 2));
}

Point Point::operator =(const Point &p) {
    this->x = p.x;
    this->y = p.y;

    return *this;
}

Point Point::operator +(const Point &p) {
    return Point(this->x + p.x, this->y + p.y);
}

bool operator ==(const Point &p1, const Point &p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}

bool operator !=(const Point &p1, const Point &p2) {
    return !(p1 == p2);
}
