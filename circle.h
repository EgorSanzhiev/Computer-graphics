#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QImage>
#include <iserializable.h>

class Circle : public ISerializable {
public:
    Circle(int x = 0, int y = 0, int r = 20);
    void draw(QImage* pBackBuffer);

    void setX(int x);
    int getX();

    void setY(int y);
    int getY();

    void setR(int r);
    int getR();

    virtual void read(QJsonObject settings);
    virtual QJsonObject serialize();
private:
    int x, y, r;
};
#endif // CIRCLE_H
