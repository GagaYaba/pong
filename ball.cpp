#include "Ball.h"
#include "Paddle.h"
#include "Game.h"
#include <QGraphicsScene>
#include <QList>

Ball::Ball(int screenWidth, int screenHeight, Game* game, QObject* parent)
    : QObject(parent), screenWidth(screenWidth), screenHeight(screenHeight), game(game) {

    setRect(0, 0, 10, 10);
    setPos(screenWidth / 2, screenHeight / 2);

    dx = 2;
    dy = 0;
    direction = -1;

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Ball::move);
    movementTimer->start(16);

    timer.start();
}

void Ball::move() {
    deltaTime = timer.elapsed() / 16.0f;
    timer.restart();

    setX(x() + dx * direction * deltaTime);
    setY(y() + dy * deltaTime);

    handleWallCollision();
    handlePaddleCollision();
    handleBoundary();
}

void Ball::handleWallCollision() {
    if (y() <= 0 || y() + rect().height() >= screenHeight) {
        dy = -dy;
    }
}

void Ball::handlePaddleCollision() {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        Paddle* paddle = dynamic_cast<Paddle*>(item);
        if (paddle) {
            dx = -dx;
            float hitPosition = (y() + rect().height() / 2) - (paddle->y() + paddle->rect().height() / 2);
            dy += hitPosition * 0.1;
        }
    }
}

void Ball::handleBoundary() {
    if (x() <= 0) {
        game->increasePlayer2Score();
        setPos(screenWidth / 2, screenHeight / 2);
        direction = -direction;
        dx = 2;
        dy = 0;
    } else if (x() + rect().width() >= screenWidth) {
        game->increasePlayer1Score();
        setPos(screenWidth / 2, screenHeight / 2);
        direction = -direction;
        dx = 2;
        dy = 0;
    }
}
