#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QImage>

class Circle {
public:
    explicit Circle();
    void draw(QImage* pBackBuffer);
    void setX(int x);
    void setY(int y);
    void setR(int r);
    void save(QString &path);
private:
    int x, y, r;
    QImage *backBuffer;
};
#endif // CIRCLE_H
