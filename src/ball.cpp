#include "../include/ball.h"
#include "../include/boundary.h"
#include "../include/paddle.h"
#include "../include/game.h"
#include <QGraphicsScene>
#include <QList>
#include <QDebug>

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

    handleCollisions();
}

void Ball::handleCollisions() {
    handlePaddleCollision();
    handleBoundaryCollision();
}

void Ball::handlePaddleCollision() {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        Paddle* paddle = dynamic_cast<Paddle*>(item);
        if (paddle) {
            handlePaddleHit(paddle);
            reverseXDirection();
            adjustVerticalSpeed(paddle);
            adjustSpeed();
            limitMaxSpeed();
        }
    }
}

void Ball::handlePaddleHit(Paddle* paddle) {
    if (paddle == game->getPlayer1()) {
        lastTouchedByPlayer = P1;
    } else if (paddle == game->getPlayer2()) {
        lastTouchedByPlayer = P2;
    }
}

void Ball::reverseXDirection() {
    dx = -dx;
}

void Ball::adjustVerticalSpeed(Paddle* paddle) {
    float hitPosition = (y() + rect().height() / 2) - (paddle->y() + paddle->rect().height() / 2);
    dy += hitPosition * 0.1;
}

void Ball::adjustSpeed() {
    // if need to reduce ball speed after condition
}

void Ball::limitMaxSpeed() {
    float maxSpeed = 6.0f;
    if (std::abs(dx) > maxSpeed) {
        dx = (dx > 0 ? 1 : -1) * maxSpeed;
    }
    if (std::abs(dy) > maxSpeed) {
        dy = (dy > 0 ? 1 : -1) * maxSpeed;
    }
}

void Ball::handleBoundaryCollision() {
    if (y() <= 0 || y() + rect().height() >= screenHeight) {
        dy = -dy;
    }

    if (x() <= 0 || x() + rect().width() >= screenWidth) {
        processGoal();
        resetBall();
    }
}

void Ball::processGoal() {
    if (x() <= 0) {
        if (lastTouchedByPlayer == P1) {
            qDebug() << "P1 OWN GOAL / NO POINT";
        } else if (lastTouchedByPlayer == P2) {
            game->increasePlayer2Score();
            qDebug() << "P2 +1 POINT";
        }
    } else {
        if (lastTouchedByPlayer == P2) {
            qDebug() << "P2 OWN GOAL / NO POINT";
        } else if (lastTouchedByPlayer == P1) {
            game->increasePlayer1Score();
            qDebug() << "P1 +1 POINT";
        }
    }
}

void Ball::resetBall() {
    setPos(screenWidth / 2, screenHeight / 2);
    direction = -direction;
    dx = 2;
    dy = 0;
}
