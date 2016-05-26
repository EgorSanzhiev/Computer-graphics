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

void Controller::setFocus1X(int x) {
    lemniscate->setFocus1X(x);

    emit modelUpdated();
}

void Controller::setFocus1Y(int y) {
    lemniscate->setFocus1Y(y);

    emit modelUpdated();
}

void Controller::setFocus2X(int x) {
    lemniscate->setFocus2X(x);

    emit modelUpdated();
}

void Controller::setFocus2Y(int y) {
    lemniscate->setFocus2Y(y);

    emit modelUpdated();
}

void Controller::drawLemniscate(QImage *buffer) {
    lemniscate->draw(buffer);
}

void Controller::loadCircleFromJson(QString &filename) {
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

//    lemniscate->read(circleSettings.toObject());

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

//    QJsonObject settings = lemniscate->serialize();

//    QJsonDocument settingsDoc(settings);

//    settingsFile.write(settingsDoc.toJson());
}
