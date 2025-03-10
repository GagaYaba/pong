#include "../include/game.h"

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    int screenWidth = 400;
    int screenHeight = 400;

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    setScene(scene);

    keysPressed = new QSet<int>();

    player1 = new Paddle(30, 150, 5, screenHeight, keysPressed, Paddle::P1);
    player2 = new Paddle(360, 150, 5, screenHeight, keysPressed, Paddle::P2);

    player1->setKeys(Qt::Key_S, Qt::Key_W);
    player2->setKeys(Qt::Key_Up, Qt::Key_Down);

    scene->addItem(player1);
    scene->addItem(player2);

    ball = new Ball(screenWidth, screenHeight, this);
    scene->addItem(ball);

    score = new Score(120, 10);
    scene->addItem(score);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFixedSize(screenWidth, screenHeight);
    setFocus();
}

void Game::increasePlayer1Score() {
    score->increasePlayer1Score();
}

void Game::increasePlayer2Score() {
    score->increasePlayer2Score();
}

void Game::keyPressEvent(QKeyEvent *event) {
    keysPressed->insert(event->key());
}

void Game::keyReleaseEvent(QKeyEvent *event) {
    keysPressed->remove(event->key());
}

Paddle* Game::getPlayer1() {
    return player1;
}

Paddle* Game::getPlayer2() {
    return player2;
}


void Game::closeEvent(QCloseEvent *event) {
    emit gameClosed();
    event->accept();
}
