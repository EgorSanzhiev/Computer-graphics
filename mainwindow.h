#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawwidget.h"
#include "controlspanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void saveFile();

private:
    QGroupBox *drawPanel;
    ControlsPanel *controlsPanel;
    DrawWidget *drawWidget;
    QSlider *sliderX;
    QWidget *centralWidget;
    QMenu *fileMenu;
    QAction *saveFileAction;

    void setupDrawPanel();
};

#endif // MAINWINDOW_H
