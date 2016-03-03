#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QImage>

class Circle {
public:
    Circle(int x = 0, int y = 0, int r = 20);
    void draw(QImage* pBackBuffer);
    void setX(int x);
    void setY(int y);
    void setR(int r);
private:
    int x, y, r;
};
#endif // CIRCLE_H
