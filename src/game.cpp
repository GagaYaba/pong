#include "../include/game.h"

Game::Game(QWidget *parent, GameMode mode)
    : QGraphicsView(parent), gameMode(mode) {
    int screenWidth = 600;
    int screenHeight = 600;

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    setScene(scene);

    keysPressed = new QSet<int>();

    setupPlayersAndPaddles();

    ball = new Ball(screenWidth, screenHeight, this);
    scene->addItem(ball);

    score = new Score(120, 10);
    scene->addItem(score);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFixedSize(screenWidth, screenHeight);
    setFocus();
}

void Game::setupPlayersAndPaddles() {
    int screenWidth = 600;
    int screenHeight = 600;

    if (gameMode == OneVOne) {
        players.append(new Player(new Paddle(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P1), Qt::Key_S, Qt::Key_W));
        players.append(new Player(new Paddle(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P2), Qt::Key_Up, Qt::Key_Down));
    }

    else if (gameMode == TwoVTwo) {
        players.append(new Player(new Paddle(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P1), Qt::Key_A, Qt::Key_Q));
        players.append(new Player(new Paddle(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P2), Qt::Key_Up, Qt::Key_Down));
        players.append(new Player(new Paddle(screenWidth * 0.2, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::P3), Qt::Key_D, Qt::Key_C));
        players.append(new Player(new Paddle(screenWidth * 0.8 - 10, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::P4), Qt::Key_8, Qt::Key_5));
    }

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
