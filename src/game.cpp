// game.cpp
#include <QMessageBox>
#include "../include/game.h"

Game::Game(QWidget *parent, GameMode mode)
    : QGraphicsView(parent), gameMode(mode) {
    int screenWidth = 600;
    int screenHeight = 600;

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    setScene(scene);

    keysPressed = new QSet<int>();

    // Configure les joueurs et les paddles en fonction du mode de jeu
    setupPlayersAndPaddles("OneVOne");

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

void Game::setupPlayersAndPaddles(QString role) {
    int screenWidth = 600;
    int screenHeight = 600;

    // Mode 1v1
    if (role == "OneVOne") {
        players.append(new Player(new Paddle(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P1), Qt::Key_S, Qt::Key_W, keysPressed));
        players.append(new Player(new Paddle(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P2), Qt::Key_Up, Qt::Key_Down, keysPressed));
    }
    else if (role == "TwoVTwo") {
        players.append(new Player(new Paddle(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P1), Qt::Key_S, Qt::Key_W, keysPressed));
        players.append(new Player(new Paddle(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P2), Qt::Key_Up, Qt::Key_Down, keysPressed));
        players.append(new Player(new Paddle(screenWidth * 0.2, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::P3), Qt::Key_R, Qt::Key_D, keysPressed));
        players.append(new Player(new Paddle(screenWidth * 0.8 - 10, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::P4), Qt::Key_Left, Qt::Key_Right, keysPressed));
    }

    // Ajoute les paddles à la scène
    for (Player* player : players) {
        scene->addItem(player->getPaddle());
    }
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