#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QImage>

class Circle {
public:
    explicit Circle();
    void draw(QImage* pBackBuffer);
    void repaint();
    void setX(int x);
    void setY(int y);
    void setR(int r);
private:
    int x, y, r;
};
#endif // CIRCLE_H
