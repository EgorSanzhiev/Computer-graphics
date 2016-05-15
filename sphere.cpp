#include "sphere.h"
#include <cmath>
#include <iostream>

int Sphere::radius = 256;

void Sphere::setPixel(QImage *buffer, int x, int y, unsigned int color)
{
    if (x < 0 || x >= buffer->width() || y < 0 || y >= buffer->height())
        return;

    uchar *backBuffer = buffer->bits();

    int bytesPerLine = buffer->bytesPerLine();

    int depth = buffer->depth() / 8;

    *(backBuffer + bytesPerLine * y + x * depth) = qRed(color);
    *(backBuffer + bytesPerLine * y + x * depth + 1) = qGreen(color);
    *(backBuffer + bytesPerLine * y + x * depth + 2) = qBlue(color);
}


QRgb Sphere::bilinearFiltering(QImage *source, double u, double v)
{
    int width = source->width();
    int height = source->height();
    double temp;

    int shiftX = (this->shiftX > 0) ? (this->shiftX) : (360 + this->shiftX);
    int shiftY = (this->shiftY > 0) ? (this->shiftY) : (360 + this->shiftY);

    double x = u * width + width * double(shiftX) / 360;
    double y = v * height + height * double(shiftY) / 360;

    int floorX = int(x) % width;
    int floorY = int(y) % height;

    double u_ratio = modf(std::abs(floorX - x), &temp);
    double v_ratio = modf(std::abs(floorY - y), &temp);
    double u_opposite = 1 - u_ratio;
    double v_opposite = 1 - v_ratio;

    uint upperLeft, lowerLeft, upperRight, lowerRight;
    int lowerLeftY, upperRightX, lowerRightX, lowerRightY;

    lowerLeftY = (floorY + 1) % height;
    lowerRightX = (floorX + 1) % width;
    lowerRightY = (floorY + 1) % height;
    upperRightX = (floorX + 1) % width;

    upperLeft = source->pixel(floorX, floorY);
    lowerLeft = source->pixel(floorX, lowerLeftY);
    lowerRight = source->pixel(lowerRightX, lowerRightY);
    upperRight = source->pixel(upperRightX, floorY);

    int red = round((qRed(upperLeft) * u_opposite + qRed(upperRight) * u_ratio) * v_opposite +
            (qRed(lowerLeft) * u_opposite + qRed(lowerRight) * u_ratio) * v_ratio);

    int green = round((qGreen(upperLeft) * u_opposite + qGreen(upperRight) * u_ratio) * v_opposite +
            (qGreen(lowerLeft) * u_opposite + qGreen(lowerRight) * u_ratio) * v_ratio);

    int blue = round((qBlue(upperLeft) * u_opposite + qBlue(upperRight) * u_ratio) * v_opposite +
            (qBlue(lowerLeft) * u_opposite + qBlue(lowerRight) * u_ratio) * v_ratio);

    QRgb result = qRgb(red, green, blue);

    return result;
}

void Sphere::read(const QJsonObject &settings)
{
    QJsonValue xValue = settings["x"];
    QJsonValue yValue = settings["y"];
    QJsonValue scaleValue = settings["scale"];
    QJsonValue sourceValue = settings["source"];
    QJsonValue filteringValue = settings["filter"];

    if (!xValue.isDouble() || !yValue.isDouble() || !scaleValue.isDouble() || !sourceValue.isString() || !filteringValue.isString())
        throw ISerializable::ParserException();

    int x = xValue.toInt();
    int y = yValue.toInt();
    int scale = scaleValue.toInt();
    QString source = sourceValue.toString();
    QString filtering = filteringValue.toString();

    if (abs(x) > 360 || abs(y) > 360 || abs(scale) > 1000)
        throw ISerializable::ParserException();

    this->setX(x);
    this->setY(y);
    this->setScale(scale);
    this->setImageFilename(source);

    if (filtering == "nearest")
        this->applyNearestFiltering();
    else if (filtering == "bilinear")
        this->applyBilinearFiltering();

}

QJsonObject Sphere::serialize()
{

}

Sphere::Sphere()
{
    this->shiftY = 0;
    this->shiftX = 0;
    this->scale = 1.0;
    filteringType = NEAREST;
    source = new QImage();
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

void Sphere::setScale(int scale)
{
    this->scale = scale > 0 ? (1.0 + (double) scale / 100.0) : (1.0 + (double) scale / 1000.0);
}

void Sphere::applyBilinearFiltering()
{
    filteringType = BILINEAR;
}

void Sphere::applyNearestFiltering()
{
    filteringType = NEAREST;
}

void Sphere::setImageFilename(QString &filename)
{
    if (filename.isEmpty())
        return;

    this->imageFilename = filename;
    source = new QImage(imageFilename);
}

void Sphere::draw(QImage *buffer) {
    if (source->isNull()) {
        std::cerr << "ERROR: wrong image" << std::endl;
        return;
    }

    int width = source->width();
    int height = source->height();

    int r = radius * scale;

    int minX = -r;
    int maxX = r;

    if (r > int(buffer->width() / 2)) {
        minX = -buffer->width() / 2;
        maxX = buffer->width() / 2 - 1;
    }

    int shiftX = (this->shiftX > 0) ? (this->shiftX) : (360 + this->shiftX);
    int shiftY = (this->shiftY > 0) ? (this->shiftY) : (360 + this->shiftY);

    for (int x = minX; x <= maxX; ++x) {
        int minZ = round(-sqrt(r * r - x * x));
        int maxZ = round(sqrt(r * r - x * x));
        for (int z = minZ; z <= maxZ; ++z) {
            if (x * x + z * z > r * r)
                continue;

            int y = -round(sqrt(r * r - x * x - z * z));

            double theta = acos(double(z) / r);
            double phi = acos(x / sqrt(x * x + y * y));

            if (x * x + y * y == 0) {
                phi = M_PI_2;
                theta = 0;
            }

            double u = phi / (2 * M_PI);
            double v = theta / M_PI;

            int textureX = int(round(u * width + width * double(shiftX) / 360)) % width;
            int textureY = int(round(v * height + height * double(shiftY) / 360)) % height;

            QRgb color;

            if (filteringType == NEAREST)
                color = source->pixel(textureX, textureY);
            else
                color = bilinearFiltering(source, u, v);

//            double screenPos = -r;
//            double watchPos = -4 * r;

//            int screenX = buffer->width() / 2 + round((watchPos - screenPos) * (-x) / (watchPos - y));
//            int screenY = buffer->height() / 2 - round((watchPos - screenPos) * z / (watchPos - y));

            int screenX = buffer->width() / 2 - x;
            int screenY = buffer->height() / 2 - z;

            setPixel(buffer, screenX, screenY, color);
        }
    }
}
