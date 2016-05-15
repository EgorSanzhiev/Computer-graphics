#include "controller.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Controller* Controller::instance = NULL;

Controller::Controller(QObject *parent) : QObject(parent) {
    sphere = new Sphere();
}

Controller* Controller::getInstance() {
    if (!instance) {
        instance = new Controller();
    }

    return instance;
}

void Controller::draw(QImage *buffer) {
    sphere->draw(buffer);
}

void Controller::setX(int x) {
    sphere->setX(x);

    emit modelUpdated();
}

void Controller::setY(int y) {
    sphere->setY(y);

    emit modelUpdated();
}

void Controller::setScale(int scale) {
    sphere->setScale(scale);

    emit modelUpdated();
}

void Controller::setFilteringType(QString type)
{
    if (type == "nearest")
        sphere->applyNearestFiltering();
    else if (type == "bilinear")
        sphere->applyBilinearFiltering();

    emit modelUpdated();
}

void Controller::startDrag() {
    cacheX = sphere->getX();
    cacheY = sphere->getY();
}

void Controller::drag(int x, int y) {
    if (abs(x + cacheX) > 360 || abs(y + cacheY) > 360)
        return;

    setX(x + cacheX);
    setY(y + cacheY);

    emit modelDragged(sphere->getX(), sphere->getY());
}

void Controller::loadModelFromJson(QString &filename) {
    if (!filename.endsWith(".json") && !filename.isEmpty()) {
        throw ISerializable::ParserException();
    }

    QFile settingsFile(filename);

    QString jsonText;

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    jsonText = settingsFile.readAll();

    QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    QJsonObject settings = document.object();

    QJsonObject sphereSettings = settings["sphere"].toObject();

    delete sphere;

    sphere = new Sphere();

    sphere->read(sphereSettings);

    settingsFile.close();

    emit modelUpdated();
}

void Controller::loadPanelFromJson(QString &filename) {
    if (!filename.endsWith(".json") && !filename.isEmpty()) {
        throw ISerializable::ParserException();
    }

    QFile settingsFile(filename);

    QString jsonText;

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    jsonText = settingsFile.readAll();

    QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    QJsonObject settings = document.object();

    QJsonValue panelSettings = settings["panel"];

    QJsonValue panelSize = panelSettings.toObject()["size"];

    QJsonValue xPanel = panelSize.toObject()["x"];

    QJsonValue yPanel = panelSize.toObject()["y"];

    if (!xPanel.isDouble() || !yPanel.isDouble()) {
        throw ISerializable::ParserException();
    }

    if (xPanel.toInt() <= 0 || yPanel.toInt() <= 0) {
        throw ISerializable::ParserException();
    }

    emit panelLoaded(xPanel.toInt(), yPanel.toInt());
}

void Controller::saveJSONSettings(QString &filename) {
    if (!filename.endsWith(".json") && !filename.isEmpty()) {
        throw ISerializable::ParserException();
    }

    QFile settingsFile(filename);

    settingsFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QJsonObject settings = sphere->serialize();

    QJsonDocument settingsDoc(settings);

    settingsFile.write(settingsDoc.toJson());
}

void Controller::setImageName(QString &filename)
{
    sphere->setImageFilename(filename);

    emit modelUpdated();
}
