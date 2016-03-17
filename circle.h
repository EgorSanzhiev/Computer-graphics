#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QImage>
#include <iserializable.h>

class Circle : public QObject, public ISerializable {
    Q_OBJECT
signals:
    void settingsLoaded(int x, int y, int r);

public:
    Circle(int x = 0, int y = 0, int r = 20);
    void draw(QImage* pBackBuffer);

    void setX(int x);
    void setY(int y);
    void setR(int r);

    virtual void read(QJsonObject settings);
    virtual QJsonObject serialize();
private:
    int x, y, r;
};
#endif // CIRCLE_H
