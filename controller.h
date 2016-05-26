#ifndef CONTROLLER
#define CONTROLLER

#include "lemniscate.h"
#include <QWidget>

class Controller : public QObject {
    Q_OBJECT

public:
    static const int DEFAULT_F1X = -20, DEFAULT_F1Y = 0, DEFAULT_F2X = 20, DEFAULT_F2Y = 0;
    static const int MIN_RANGE = -1000, MAX_RANGE = 1000;
private:
    Lemniscate *lemniscate;
    static Controller *instance;
    Controller(QObject *parent = 0);

signals:
    void modelUpdated();
    void configLoaded(int focus1X, int focus1Y, int focus2X, int focus2Y);
    void panelLoaded(int xPanel, int yPanel);

public slots:
    void setFocus1X(int x);
    void setFocus1Y(int y);
    void setFocus2X(int x);
    void setFocus2Y(int y);

public:
    class ParserException{};

    static Controller *getInstance();
    void drawLemniscate(QImage *buffer);
    void loadCircleFromJson(QString &filename);
    void loadPanelFromJson(QString &filename);
    void saveJSONSettings(QString &filename);
};

#endif // CONTROLLER

