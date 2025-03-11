// game.cpp
#include <QMessageBox>
#include "../include/game.h"
#include "../include/globals.h"

Game::Game(QWidget *parent, GameMode mode)
    : QGraphicsView(parent), gameMode(mode) {
    int screenWidth = 600;
    int screenHeight = 600;

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    setScene(scene);

    keysPressed = new QSet<int>();

    // Configure les joueurs et les paddles en fonction du mode de jeu
    setupPlayersAndPaddles("OneVOne", );

    ball = new Ball(screenWidth, screenHeight, this);
    scene->addItem(ball);

    score = new Score(120, 10);
    scene->addItem(score);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Game::updateGame);
    timer->start(16); // Environ 60 FPS

    setFixedSize(screenWidth, screenHeight);
    setFocus();
}

void Game::setupPlayersAndPaddles() {
    int screenWidth = 600;
    int screenHeight = 600;

    // Vérifier quel joueur est ce client
    if (g_playerRole == "p1") {
        players.append(new Player(new Paddle(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::p1), Qt::Key_S, Qt::Key_W, keysPressed));
    } else if (g_playerRole == "p2") {
        players.append(new Player(new Paddle(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::p2), Qt::Key_Up, Qt::Key_Down, keysPressed));

    // Ajouter uniquement le paddle du joueur local à la scène
    if (!players.isEmpty()) {
        scene->addItem(players[0]->getPaddle());
    }

    // Les autres joueurs seront mis à jour via le réseau (ex: setPaddlePosition)
}

Ball* Game::getBall() {
    return ball;
}

void Game::increaseTeam1Score() {
    score->increaseTeam1Score();
}

void Game::increaseTeam2Score() {
    score->increaseTeam2Score();
}

void Game::keyPressEvent(QKeyEvent *event) {
    keysPressed->insert(event->key());
}

void Game::keyReleaseEvent(QKeyEvent *event) {
    keysPressed->remove(event->key());
}

Paddle* Game::getPaddle(int playerIndex) {
    if (playerIndex >= 0 && playerIndex < players.size()) {
        return players[playerIndex]->getPaddle();
    }
    return nullptr;
}

void Game::closeEvent(QCloseEvent *event) {
    int ret = QMessageBox::question(this, "Quitter", "Voulez-vous vraiment quitter ?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

void Game::updateGame() {
    for (Player* player : players) {
        player->update();
    }
}