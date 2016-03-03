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

void Controller::setDrawWidget(QWidget *widget) {
    this->drawWidget = widget;
}

void Controller::setX(int x) {
    circle->setX(x);
    drawWidget->repaint();
}

void Controller::setY(int y) {
    circle->setY(y);
    drawWidget->repaint();
}

void Controller::setR(int r) {
    circle->setR(r);
    drawWidget->repaint();
}

void Controller::loadJSONSettings(QString &filename) {
    QFile settingsFile(filename);

    QString jsonText;

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    jsonText = settingsFile.readAll();

    QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    QJsonObject settings = document.object();

    QJsonObject circleSettings = settings[tr("circle")].toObject();

    QJsonValue r = circleSettings[tr("R")];

    QJsonValue position = circleSettings[tr("position")];

    QJsonValue x = position.toObject()[tr("x")];

    QJsonValue y = position.toObject()[tr("y")];

    circle->setR(r.toInt(20));
    circle->setX(x.toInt());
    circle->setY(y.toInt());

    emit configLoaded(x.toInt(), y.toInt(), r.toInt(20));

    drawWidget->update();
}
