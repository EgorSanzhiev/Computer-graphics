#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QImage>

class Circle : public QObject {
    Q_OBJECT
public:
    explicit Circle(QObject* parent = 0);
    void draw(QImage* pBackBuffer);
private:
    int x, y, r;
};
#endif // CIRCLE_H
