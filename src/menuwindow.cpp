#include "../include/menuwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

MenuWindow::MenuWindow(QWidget *parent) : QMainWindow(parent) {
    createMenu();
}

MenuWindow::~MenuWindow() {}

void MenuWindow::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("Menu");

    QAction *startAction = new QAction("Démarrer", this);
    QAction *quitAction = new QAction("Quitter", this);

    connect(startAction, &QAction::triggered, this, &MenuWindow::onStart);
    connect(quitAction, &QAction::triggered, this, &MenuWindow::onQuit);

    fileMenu->addAction(startAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
}

void MenuWindow::onStart() {
    if (!mainWindow) {
        secondWindow = new SecondWindow(this);
    }
    secondWindow->show();
}

void MenuWindow::onQuit() {
    close();
}
