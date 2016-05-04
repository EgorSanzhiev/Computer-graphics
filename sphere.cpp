#include "sphere.h"
#include <QImage>
#include <cmath>
#include <iostream>

int Sphere::radius = 180;

void Sphere::read(const QJsonObject &settings)
{

}

QJsonObject Sphere::serialize()
{

}

Sphere::Sphere()
{
    this->shiftY = 0;
    this->shiftX = 0;
}

void Sphere::setX(int x)
{
    this->shiftX = x;
}

void Sphere::setY(int y)
{
    this->shiftY = y;
}

int Sphere::getX()
{
    return shiftX;
}

int Sphere::getY()
{
    return shiftY;
}

void Sphere::setImageFilename(QString &filename)
{
    this->imageFilename = filename;
}

void Sphere::draw(QImage *buffer) {
    QImage source(imageFilename);

    int width = source.width();
    int height = source.height();

    for (int i = 0; i < width; ++i) {
        int textureX = (i + shiftX) % width;

        double u = 2.0 * textureX / (double) width - 1;

        for (int j = 0; j < height; ++j) {
            int textureY = (j + shiftY) % height;

            double v = 1 - 2.0 * textureY / (double) height;

            double phi = atan(v / u) + M_PI_2;

            if (u < 0) {
                phi += M_PI;
            }

            double theta = asin(sqrt(u * u + v * v));

            double x = radius * sin(theta) * sin(phi);

            double y = radius * sin(theta) * cos(phi);

            double z = radius * cos(theta);

            double screenZ = radius;
            double watchZ = 2 * radius;

            int screenX = buffer->width() / 2 + round((watchZ - screenZ) * x / (watchZ - z));
            int screenY = buffer->height() / 2 - round((watchZ - screenZ) * (-y) / (watchZ - z));

            if (screenX < 0 || screenX >= buffer->width() || screenY < 0 || screenY >= buffer->height())
                continue;

            buffer->setPixel(screenX, screenY, source.pixel(i, j));
        }
    }
}
