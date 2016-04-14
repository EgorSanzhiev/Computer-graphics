#include "controller.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Controller* Controller::instance = NULL;

Controller::Controller(QObject *parent) : QObject(parent) {
    figure = new Figure();

    connect(figure, SIGNAL(settingsLoaded(int,int,int)), this, SIGNAL(configLoaded(int,int,int)));
}

Controller* Controller::getInstance() {
    if (!instance) {
        instance = new Controller();
    }

    return instance;
}

void Controller::draw(QImage *buffer) {
    figure->draw(buffer);
}

void Controller::setX(int x) {
    figure->setX(x);

    emit modelUpdated();
}

void Controller::setY(int y) {
    figure->setY(y);

    emit modelUpdated();
}

void Controller::setScale(int scale) {
    figure->setScale(scale);

    emit modelUpdated();
}

void Controller::setFillMode(int mode)
{
    if (mode == Qt::Checked) {
        figure->setFillMode(true);
    } else if (mode == Qt::Unchecked) {
        figure->setFillMode(false);
    }

    emit modelUpdated();
}

void Controller::setOutlineMode(int mode)
{
    if (mode == Qt::Checked) {
        figure->setOutlineMode(true);
    } else if (mode == Qt::Unchecked) {
        figure->setOutlineMode(false);
    }

    emit modelUpdated();
}

void Controller::startDrag() {
    cacheX = figure->getX();
    cacheY = figure->getY();
}

void Controller::drag(int x, int y) {
    setX(x + cacheX);
    setY(y + cacheY);

    emit modelDragged(figure->getX(), figure->getY());
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

//    QJsonValue circleSettings = settings["circle"];

    figure->read(settings);

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

    QJsonObject settings = figure->serialize();

    QJsonDocument settingsDoc(settings);

    settingsFile.write(settingsDoc.toJson());
}
