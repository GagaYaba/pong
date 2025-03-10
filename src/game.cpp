#include "../include/game.h"

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    int screenWidth = 400;
    int screenHeight = 400;

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    setScene(scene);

    keysPressed = new QSet<int>();

    paddles[0] = new Paddle(30, 150, 5, screenHeight, keysPressed, Paddle::P1);
    paddles[1] = new Paddle(360, 150, 5, screenHeight, keysPressed, Paddle::P2);
    paddles[2] = new Paddle(30, 250, 5, screenHeight, keysPressed, Paddle::P3);
    paddles[3] = new Paddle(360, 250, 5, screenHeight, keysPressed, Paddle::P4);

    paddles[0]->setKeys(Qt::Key_S, Qt::Key_W);
    paddles[1]->setKeys(Qt::Key_Up, Qt::Key_Down);
    paddles[2]->setKeys(Qt::Key_R, Qt::Key_D);
    paddles[3]->setKeys(Qt::Key_Left, Qt::Key_Right);

    for (int i = 0; i < 4; ++i) {
        scene->addItem(paddles[i]);
    }

    ball = new Ball(screenWidth, screenHeight, this);
    scene->addItem(ball);

    score = new Score(120, 10);
    scene->addItem(score);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFixedSize(screenWidth, screenHeight);
    setFocus();
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
    if (playerIndex >= 0 && playerIndex < 4) {
        return paddles[playerIndex];
    }
    return nullptr;
}
