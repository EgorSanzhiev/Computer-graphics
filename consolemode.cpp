#include "consolemode.h"
#include <QImage>
#include <iostream>

ConsoleMode::ConsoleMode() {
    controller = Controller::getInstance();

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
    std::string error = "Wrong arguments! Usage: <settings file (.txt)> <output file (.png)>";

    QString settingsFile(argv[1]);
    QString outputFile(argv[2]);

    if (!settingsFile.endsWith(".txt", Qt::CaseInsensitive)
            || !outputFile.endsWith(".png", Qt::CaseInsensitive)) {

        std::cerr << error << std::endl;

        return -1;
    }

    Controller *controller = Controller::getInstance();

    try {
        controller->loadJSONSettings(settingsFile);
    } catch (Controller::ParserException &e) {
        std::cerr << "Error in settings file" << std::endl;

        return -1;
    }

    controller->drawCircle(image);

    image->save(outputFile);

    return 0;
}
