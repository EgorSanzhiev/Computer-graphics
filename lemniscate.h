#ifndef LEMNISCATE
#define LEMNISCATE

#include <QObject>
#include <iserializable.h>
#include "point.h"
#include <set>


class Lemniscate : public QObject, public ISerializable {
    Q_OBJECT
private:
    Point focus1;
    Point focus2;

    Point binarSearch(Point &left, Point &right);
    Point findNextPoint(Point &currentPoint, Point &previousPoint, int direction);
    void drawHelper(Point &startPoint, Point &endPoint, QImage *backBuffer, int ul);

signals:
    void settingsLoaded(int x1, int y1, int x2, int y2);

public:
    Lemniscate(int focus1X = -20, int focus1Y = 0, int focus2X = 20, int focus2Y = 0);
    void draw(QImage *backBuffer);

    void setX1(int x1);
    void setY1(int y1);
    void setX2(int x2);
    void setY2(int y2);

    virtual void read(const QJsonObject &settings);
    virtual QJsonObject serialize();
};

#endif // LEMNISCATE

