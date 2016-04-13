#ifndef CONTROLLER
#define CONTROLLER

#include <QWidget>
#include "figure.h"

class Controller : public QObject {
    Q_OBJECT
private:
    Figure *figure;
    static Controller *instance;
    Controller(QObject *parent = 0);
    int cacheX;
    int cacheY;

signals:
    void configLoaded(int x, int y, int scale);
    void modelDragged(int x, int y);
    void panelLoaded(int xPanel, int yPanel);
    void modelUpdated();

public slots:
    void setX(int x);
    void setY(int y);
    void setScale(int scale);

public:
    class ParserException{};

    static Controller *getInstance();
    void draw(QImage *buffer);
    void loadModelFromJson(QString &filename);
    void loadPanelFromJson(QString &filename);
    void saveJSONSettings(QString &filename);
    void startDrag();
    void drag(int x, int y);
    void endDrag();
};

#endif // CONTROLLER

