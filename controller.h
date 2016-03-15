#ifndef CONTROLLER
#define CONTROLLER

#include "circle.h"
#include <QWidget>

class Controller : public QObject {
    Q_OBJECT
private:
    Circle *circle;
    static Controller *instance;
    Controller(QObject *parent = 0);

signals:
    void circleUpdated();
    void configLoaded(int x, int y, int r);
    void panelLoaded(int xPanel, int yPanel);

public slots:
    void setX(int x);
    void setY(int y);
    void setR(int r);

public:
    class ParserException{};

    static Controller *getInstance();
    void drawCircle(QImage *buffer);
    void saveImage(QString &path);
    void loadJSONSettings(QString &filename);
};

#endif // CONTROLLER

