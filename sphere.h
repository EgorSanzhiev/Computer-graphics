#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>
#include "iserializable.h"

class Sphere : public QObject, public ISerializable {
    Q_OBJECT

private:
    int shiftX;
    int shiftY;
    int scale;
    static int radius;

    QString imageFilename;

public:    
    void read(const QJsonObject &settings);
    QJsonObject serialize();
    
    Sphere();
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    void setScale(int scale);
    void setImageFilename(QString &filename);
    void draw(QImage *buffer);
};

#endif // SPHERE_H

