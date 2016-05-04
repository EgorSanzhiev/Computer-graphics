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
    sphere->setY(-y);

    emit modelUpdated();
}

void Controller::setScale(int scale) {
//    sphere->setScale(scale);

    emit modelUpdated();
}

void Controller::startDrag() {
    cacheX = sphere->getX();
    cacheY = sphere->getY();
}

void Controller::drag(int x, int y) {
    setX(x + cacheX);
    setY(-y + cacheY);

    emit modelDragged(sphere->getX(), sphere->getY());
}

void Controller::loadModelFromJson(QString &filename) {
//    if (!filename.endsWith(".json") && !filename.isEmpty()) {
//        throw ISerializable::ParserException();
//    }

//    QFile settingsFile(filename);

//    QString jsonText;

//    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

//    jsonText = settingsFile.readAll();

//    QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

//    QJsonObject settings = document.object();

//    delete figure;

//    figure = new Figure();

//    connect(figure, SIGNAL(settingsLoaded(int,int,int, bool, bool)), this, SIGNAL(configLoaded(int,int,int, bool, bool)));

//    sphere->read(settings);

//    settingsFile.close();

//    emit modelUpdated();
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
}
