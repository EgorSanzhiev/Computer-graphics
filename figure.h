#ifndef FIGURE
#define FIGURE

#include <QObject>
#include <QImage>
#include "iserializable.h"

class Figure : public QObject, public ISerializable {
    Q_OBJECT
private:
    struct Point {
        Point(int x = 0, int y = 0, bool onCurve = false);
        int x;
        int y;
        bool onCurve;
    };

    static const int MAX_SHIFT = 1000;
    static const int MIN_SHIFT = -1000;

    Point screenCenter;
    Point shift;
    std::vector<Point> *points;
    double scale;

    static const int steps = 1000;

    int factorial(int n);

    void setPixel(QImage *buffer, int x, int y, int color);

    double bernstein(int n, int i, double t);
    void drawBezier(QImage *buffer, Point &start, Point &end, Point &intermediate);
    void drawLine(QImage *buffer, Point &start, Point &end);

signals:
    void settingsLoaded(int x, int y, int scale);

public:
    Figure();
    ~Figure();
    void draw(QImage *buffer);
    void read(const QJsonObject &settings);
    QJsonObject serialize();
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    void setScale(int scale);
};

#endif // FIGURE

