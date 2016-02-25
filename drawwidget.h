#ifndef DRAWWIDGET
#define DRAWWIDGET

#include <QWidget>
#include "circle.h"

class DrawWidget : public QWidget {
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    ~DrawWidget();
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    Circle *circle;

};

#endif // DRAWWIDGET

