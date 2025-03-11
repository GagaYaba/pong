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
            qDebug() << "Ball hit the paddle!";
            emit paddleHit();
        }
    }
}

void Ball::handleBoundaryCollision() {
    if (y() <= 0 || y() + rect().height() >= screenHeight) {
        dy = -dy;
        qDebug() << "Ball hit the boundary!";
        emit boundaryHit();
    }

    if (x() <= 0 || x() + rect().width() >= screenWidth) {
        processGoal();
        resetBall();
    }
}

void Ball::processGoal() {
    if (hit) {
        if (x() <= 0) {
            if (lastTouchedByPlayer == P1 || lastTouchedByPlayer == P3) {
                qDebug() << "TEAM 1 OWN GOAL / NO POINT";
            } else if (lastTouchedByPlayer == P2 || lastTouchedByPlayer == P4) {
                game->increaseTeam2Score();
                qDebug() << "TEAM 2 +1 POINT";
            }
        } else {
            if (lastTouchedByPlayer == P2 || lastTouchedByPlayer == P4) {
                qDebug() << "TEAM 2 OWN GOAL / NO POINT";
            } else if (lastTouchedByPlayer == P1 || lastTouchedByPlayer == P3) {
                game->increaseTeam1Score();
                qDebug() << "TEAM 1 +1 POINT";
            }
        }
    }
}

void Ball::resetBall() {
    setPos(screenWidth / 2, screenHeight / 2);
    direction = -direction;
    dx = 2;
    dy = 0;
    hit = false;
}

void Ball::handlePaddleHit(Paddle* paddle) {
    if (!hit) {
        hit = true;
    }

    if (paddle == game->getPaddle(0) || paddle == game->getPaddle(2)) {
        lastTouchedByPlayer = (paddle == game->getPaddle(0)) ? P1 : P3;
    } else if (paddle == game->getPaddle(1) || paddle == game->getPaddle(3)) {
        lastTouchedByPlayer = (paddle == game->getPaddle(1)) ? P2 : P4;
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
    float maxSpeed = 8.0f;
    if (std::abs(dx) > maxSpeed) {
        dx = (dx > 0 ? 1 : -1) * maxSpeed;
    }
    if (std::abs(dy) > maxSpeed) {
        dy = (dy > 0 ? 1 : -1) * maxSpeed;
    }
}
