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
        Point scaleAndShift(double scale, Point shift);
    };

    static const int MAX_SHIFT = 1000;
    static const int MIN_SHIFT = -1000;

    Point screenCenter;
    Point shift;
    bool fillMode;
    bool outlineMode;
    std::vector<Point> *points;
    std::vector<std::vector<int>> *fillBorders;
    double scale;

    void setPixel(QImage *buffer, int x, int y, int color);
    void setPixel(QImage *buffer, int x, int y, int red, int green, int blue);

    double distance(double x1, double y1, double x2, double y2);
    void drawBezier(QImage *buffer, Point &start, Point &end, Point &intermediate);
    void drawLine(QImage *buffer, Point &start, Point &end);
    void fill(QImage *buffer);
    void putInFillVector(int x, int y, int yMax);

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
    void setFillMode(bool fillMode);
    void setOutlineMode(bool outlineMode);
    void setScale(int scale);
};

#endif // FIGURE

