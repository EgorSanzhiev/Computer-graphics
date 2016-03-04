#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (argc == 1) {
        MainWindow w;
        w.show();

        return a.exec();
    } else if (argc == 3){
        QString settingsFile(argv[1]);
        QString outputFile(argv[2]);

        std::string error = "Wrong arguments! Usage: <settings file (.txt)> <output file (.png)>";

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

        controller->saveImage(outputFile);
    }
}
