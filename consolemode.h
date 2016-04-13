#ifndef CONSOLEMODE
#define CONSOLEMODE

#include <QObject>
#include "controller.h"

class ConsoleMode : public QObject {
    Q_OBJECT

private:
    Controller *controller;
    QImage *image;

public slots:
    void setPanel(int width, int height);

public:
    ConsoleMode();
    ~ConsoleMode();
    int runFromConsole(int argc, char* argv[]);
};

#endif // CONSOLEMODE

