#ifndef CONTROLLER
#define CONTROLLER

#include "circle.h"
#include <QWidget>

class Controller : public QObject {
    Q_OBJECT
private:
    Circle *circle;
    QWidget *drawWidget;
    QImage *backBuffer;
    static Controller *instance;
    Controller(QObject *parent = 0);

signals:
    void configLoaded(int x, int y, int r);

public slots:
    void setX(int x);
    void setY(int y);
    void setR(int r);

public:
    class ParserException{};

    static Controller *getInstance();
    void drawCircle(QImage *buffer);
    void saveImage(QString &path);
    void setDrawWidget(QWidget *drawWidget);
    void loadJSONSettings(QString &filename);
};

#endif // CONTROLLER

