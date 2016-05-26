#include "consolemode.h"
#include <QImage>
#include <iostream>
#include "iserializable.h"

ConsoleMode::ConsoleMode() {
    controller = Controller::getInstance();

    image = new QImage();

    connect(controller, SIGNAL(panelLoaded(int,int)), this, SLOT(setPanel(int,int)));
}

void ConsoleMode::setPanel(int width, int height) {
    image = new QImage(width, height, QImage::Format_RGB888);

    memset(image->bits(), 255, image->byteCount());
}

ConsoleMode::~ConsoleMode() {
    delete image;
}

int ConsoleMode::runFromConsole(int argc, char *argv[]) {
    QString settingsFile(argv[1]);
    QString outputFile(argv[2]);

    Controller *controller = Controller::getInstance();

    try {
        controller->loadCircleFromJson(settingsFile);
        controller->loadPanelFromJson(settingsFile);
    } catch (ISerializable::ParserException &e) {
        std::cerr << "Error in settings file" << std::endl;

        return -1;
    }

    controller->drawLemniscate(image);

    if (!image->save(outputFile)) {
        std::cerr << "Couldn't save image!" << std::endl;
        return -1;
    }

    return 0;
}
