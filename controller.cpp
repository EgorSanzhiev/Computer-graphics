#include "controller.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Controller* Controller::instance = NULL;

Controller::Controller(QObject *parent) : QObject(parent) {
    circle = new Circle();
    connect(circle, SIGNAL(settingsLoaded(int,int,int)), this, SIGNAL(configLoaded(int,int,int)));
}

Controller* Controller::getInstance() {
    if (!instance) {
        instance = new Controller();
    }

    return instance;
}

void Controller::drawCircle(QImage *buffer) {
    circle->draw(buffer);
}

void Controller::setX(int x) {
    circle->setX(x);

    emit circleUpdated();
}

void Controller::setY(int y) {
    circle->setY(y);

    emit circleUpdated();
}

void Controller::setR(int r) {
    circle->setR(r);

    emit circleUpdated();
}

void Controller::loadJSONSettings(QString &filename) {
    if (!filename.endsWith(".json") && !filename.isEmpty()) {
        throw ISerializable::ParserException();
    }

    QFile settingsFile(filename);

    QString jsonText;

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    jsonText = settingsFile.readAll();

    QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    QJsonObject settings = document.object();

    QJsonValue circleSettings = settings["circle"];

    QJsonValue panelSettings = settings["panel"];

    QJsonValue panelSize = panelSettings.toObject()["size"];

    QJsonValue xPanel = panelSize.toObject()["x"];

    QJsonValue yPanel = panelSize.toObject()["y"];

    circle->read(circleSettings.toObject());

    emit circleUpdated();

    if (panelSettings.isNull()) {
        return ;
    }

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

    QJsonObject settings = circle->serialize();

    QJsonDocument settingsDoc(settings);

    settingsFile.write(settingsDoc.toJson());
}
