#ifndef CONTROLLER
#define CONTROLLER

#include "lemniscate.h"
#include <QWidget>

class Controller : public QObject {
    Q_OBJECT
private:
    Lemniscate *lemniscate;
    static Controller *instance;
    Controller(QObject *parent = 0);

signals:
    void modelUpdated();
    void configLoaded(int x1, int y1, int x2, int y2);
    void panelLoaded(int xPanel, int yPanel);

public slots:
    void setX1(int x1);
    void setY1(int y1);
    void setX2(int x2);
    void setY2(int y2);

public:
    class ParserException{};

    static Controller *getInstance();
    void draw(QImage *buffer);
    void loadModelFromJson(QString &filename);
    void loadPanelFromJson(QString &filename);
    void saveJSONSettings(QString &filename);
};

#endif // CONTROLLER

