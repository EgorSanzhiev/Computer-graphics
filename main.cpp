#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "consolemode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string error = "Wrong arguments! Usage: <settings file (.txt)> <output file (.png)>";

    if (argc == 1) {
        MainWindow w;
        w.show();

        return a.exec();
    } else if (argc == 3){
        ConsoleMode console;

        return console.runFromConsole(argc, argv);
    } else {
        std::cerr << error << std::endl;

        return -1;
    }
}
