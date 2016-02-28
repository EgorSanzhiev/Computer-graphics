#include <QVBoxLayout>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupDrawPanel();

    controlsPanel = new ControlsPanel(this);

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(drawPanel, 0, 0);
    grid->addWidget(controlsPanel, 0, 1);

    centralWidget = new QWidget(this);
    centralWidget->setLayout(grid);

    setCentralWidget(centralWidget);

    resize(800, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupDrawPanel() {
    drawPanel = new QGroupBox("Draw panel", this);

    drawWidget = new DrawWidget(drawPanel);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(drawWidget);

    drawPanel->setLayout(layout);
}
