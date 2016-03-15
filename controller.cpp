#include "controller.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Controller* Controller::instance = NULL;

Controller::Controller(QObject *parent) : QObject(parent) {
    circle = new Circle();
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
    QFile settingsFile(filename);

    QString jsonText;

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    jsonText = settingsFile.readAll();

    QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    QJsonObject settings = document.object();

    QJsonValue circleSettings = settings["circle"];

    QJsonValue panelSettings = settings["panel"];

    QJsonValue rParsed = circleSettings.toObject()["R"];

    QJsonValue position = circleSettings.toObject()["position"];

    QJsonValue xParsed = position.toObject()["x"];

    QJsonValue yParsed = position.toObject()["y"];

    QJsonValue panelSize = panelSettings.toObject()["size"];

    QJsonValue xPanel = panelSize.toObject()["x"];

    QJsonValue yPanel = panelSize.toObject()["y"];

    if (!rParsed.isDouble() || !xParsed.isDouble() || !yParsed.isDouble()) {
        throw Controller::ParserException();
    }

    int x = xParsed.toInt();
    int y = yParsed.toInt();
    int r = rParsed.toInt(20);

    if (r < 0) {
        throw Controller::ParserException();
    }

    circle->setR(r);
    circle->setX(x);
    circle->setY(y);

    emit configLoaded(x, y, r);

    emit circleUpdated();

    if (panelSettings.isNull()) {
        return ;
    }

    if (!xPanel.isDouble() || !yPanel.isDouble()) {
        throw Controller::ParserException();
    }

    if (xPanel.toInt() <= 0 || yPanel.toInt() <= 0) {
        throw Controller::ParserException();
    }

    emit panelLoaded(xPanel.toInt(), yPanel.toInt());
}
