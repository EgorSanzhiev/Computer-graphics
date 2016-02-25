#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    drawWidget = new DrawWidget(this);
    setCentralWidget(drawWidget);
    resize(800, 600);
}

MainWindow::~MainWindow()
{

}
