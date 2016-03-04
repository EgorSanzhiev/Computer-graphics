#include "controller.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Controller* Controller::instance = NULL;

Controller::Controller(QObject *parent) : QObject(parent) {
    circle = new Circle();
    drawWidget = NULL;
    backBuffer = NULL;
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

void Controller::saveImage(QString &path) {
    backBuffer->save(path);
}

void Controller::loadJSONSettings(QString &filename) {
    QFile settingsFile(filename);

    QString jsonText;

    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);

    jsonText = settingsFile.readAll();

    QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    QJsonObject settings = document.object();

    QJsonObject circleSettings = settings["circle"].toObject();

    QJsonObject panelSettings = settings["panel"].toObject();

    QJsonValue rParsed = circleSettings["R"];

    QJsonValue position = circleSettings["position"];

    QJsonValue xParsed = position.toObject()["x"];

    QJsonValue yParsed = position.toObject()["y"];

    QJsonValue panelSize = panelSettings["size"];

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

    if (drawWidget) {
        drawWidget->update();
    } else {
        if (!xPanel.isDouble() || !yPanel.isDouble()) {
            throw Controller::ParserException();
        }

        if (xPanel.toInt() <= 0 || yPanel.toInt() <= 0) {
            throw Controller::ParserException();
        }

        backBuffer = new QImage(xPanel.toInt(), yPanel.toInt(), QImage::Format_RGB888);

        memset(backBuffer->bits(), 255, backBuffer->byteCount());

        drawCircle(backBuffer);
    }
}
