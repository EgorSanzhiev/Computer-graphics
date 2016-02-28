#include "controller.h"

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
