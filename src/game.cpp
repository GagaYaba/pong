#include "../include/game.h"

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    int screenWidth = 600;
    int screenHeight = 600;

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    setScene(scene);

    keysPressed = new QSet<int>();

    // Création des joueurs avec leurs paddles et touches associées
    players.append(new Player(new Paddle(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P1), Qt::Key_S, Qt::Key_W));
    players.append(new Player(new Paddle(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::P2), Qt::Key_Up, Qt::Key_Down));
    players.append(new Player(new Paddle(screenWidth * 0.2, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::P3), Qt::Key_R, Qt::Key_D));
    players.append(new Player(new Paddle(screenWidth * 0.8 - 10, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::P4), Qt::Key_Left, Qt::Key_Right));

    // Ajout des paddles des joueurs à la scène
    for (Player* player : players) {
        scene->addItem(player->getPaddle());
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
    if (playerIndex >= 0 && playerIndex < players.size()) {
        return players[playerIndex]->getPaddle();
    }
    return nullptr;
}
