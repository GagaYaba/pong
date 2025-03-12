#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief Constructeur de la classe MainWindow.
 * @param parent Le parent QWidget.
 */
MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/**
 * @brief Destructeur de la classe MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}