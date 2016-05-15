#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>
#include "iserializable.h"
#include <QImage>

class Sphere : public QObject, public ISerializable {
    Q_OBJECT

private:
    enum FilteringType{BILINEAR, NEAREST};

    int shiftX;
    int shiftY;
    double scale;
    static int radius;
    FilteringType filteringType;

    QString imageFilename;
    QImage *source;

    void setPixel(QImage *buffer, int x, int y, unsigned int color);
    QRgb bilinearFiltering(QImage *source, double u, double v);

public:    
    void read(const QJsonObject &settings);
    QJsonObject serialize();
    
    Sphere();
    void setX(int x);
    void setY(int y);
    void setScale(int scale);
    void applyBilinearFiltering();
    void applyNearestFiltering();
    int getX();
    int getY();
    void setImageFilename(QString &filename);
    void draw(QImage *buffer);
};

#endif // SPHERE_H

