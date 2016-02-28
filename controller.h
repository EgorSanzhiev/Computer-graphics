#ifndef CONTROLLER
#define CONTROLLER

#include "circle.h"
#include <QWidget>

class Controller : public QObject {
    Q_OBJECT
private:
    Circle *circle;
    QWidget *drawWidget;
    static Controller *instance;
    Controller(QObject *parent = 0);

public slots:
    void setX(int x);
    void setY(int y);
    void setR(int r);

public:
    static Controller *getInstance();
    void drawCircle(QImage *buffer);
    void setDrawWidget(QWidget *drawWidget);
};

#endif // CONTROLLER

