#include "../include/menuwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHostAddress>

MenuWindow::MenuWindow(QWidget *parent)
        : QMainWindow(parent), game(nullptr), server(nullptr), client(nullptr), ui(new Ui::MenuWindow) {
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &MenuWindow::onStart);
    connect(ui->quitButton, &QPushButton::clicked, this, &MenuWindow::onQuit);
}

MenuWindow::~MenuWindow() {
    delete game;
    delete server;
    delete client;
    delete ui;
}

void MenuWindow::onStart() {
    if (!server) {
        server = new GameServer(this);
        server->startServer(12345, 1, false);
    }

    if (!client) {
        client = new GameClient(this);
        client->connectToServer(QHostAddress::LocalHost, 12345);
    }

    if (!game) {
        game = new Game();
        connect(game, &Game::gameClosed, this, &MenuWindow::onGameClosed);
    }

    hide();
    game->show();
}

void MenuWindow::onQuit() {
    close();
}

// Slot pour afficher le menu lorsque le jeu est fermé
void MenuWindow::onGameClosed() {
    show();  // Affiche le menu
    game->deleteLater();  // Supprimer l'instance du jeu
    game = nullptr;  // Réinitialise l'objet jeu
}
