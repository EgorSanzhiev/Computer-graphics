#include "controller.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Controller* Controller::instance = NULL;

Controller::Controller(QObject *parent) : QObject(parent) {
    lemniscate = new Lemniscate();

    connect(lemniscate, SIGNAL(settingsLoaded(int,int,int,int)), this, SIGNAL(configLoaded(int,int,int,int)));
}

Controller* Controller::getInstance() {
    if (!instance) {
        instance = new Controller();
    }

    return instance;
}

void Controller::draw(QImage *buffer) {
    lemniscate->draw(buffer);
}

void Controller::setX1(int x1) {
    lemniscate->setX1(x1);

    emit modelUpdated();
}

void Controller::setY1(int y1) {
    lemniscate->setY1(y1);

    emit modelUpdated();
}

void Controller::setX2(int x2) {
    lemniscate->setX2(x2);

    emit modelUpdated();
}

void Controller::setY2(int y2) {
    lemniscate->setY2(y2);

    emit modelUpdated();
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

    QJsonValue circleSettings = settings["circle"];

    lemniscate->read(circleSettings.toObject());

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

    QJsonObject settings = lemniscate->serialize();

    QJsonDocument settingsDoc(settings);

    settingsFile.write(settingsDoc.toJson());
}
