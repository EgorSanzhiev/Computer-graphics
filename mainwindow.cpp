#include <QVBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QErrorMessage>
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

    saveFileAction = new QAction(tr("Save"), this);
    saveFileAction->setShortcut(QKeySequence::Save);
    connect(saveFileAction, SIGNAL(triggered(bool)), this, SLOT(saveFile()));

    loadFileAction = new QAction(tr("Load"), this);
    loadFileAction->setShortcut(QKeySequence::Open);
    connect(loadFileAction, SIGNAL(triggered(bool)), this, SLOT(loadSettings()));

    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(loadFileAction);

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

void MainWindow::saveFile() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Choose file"), "/home/egorsanzhiev", tr("Images (*.png)"));

    drawWidget->saveImage(filename);
}

void MainWindow::loadSettings() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file"), "/home/egorsanzhiev", tr("Text (*.txt)"));

    try {
        Controller::getInstance()->loadJSONSettings(filename);
    } catch (const Controller::ParserException &e) {
        QErrorMessage *errMsg = new QErrorMessage(this);
        errMsg->showMessage(tr("Error in the settings file"));
    }
}
