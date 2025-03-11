#include "../include/paddle.h"

#include <QKeyEvent>
#include <QDebug>

Paddle::Paddle(int x, int y, int speed, int screenHeight, QSet<int>* keysPressed, Player playerId, QObject* parent)
    : QObject(parent), speed(speed), screenHeight(screenHeight), keysPressed(keysPressed), playerId(playerId) {
    setRect(0, 0, 10, 60);
    qDebug() << x << " P "<< y;
    setPos(x, y);

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Paddle::updatePosition);
    movementTimer->start(16);
}

PaddleNetwork::PaddleNetwork(int x, int y, int speed, int screenHeight, Player playerId, QObject* parent)
    : QObject(parent), speed(speed), screenHeight(screenHeight), playerId(playerId) {
    qDebug() << x << "  "<< y;
    qDebug() << "PaddleNetwork created";
    setRect(0, 0, 10, 60);
    setPos(x, y);
}

void PaddleNetwork::updatePosition(float y) {
    setY(y);
}

void Paddle::setKeys(int upKey, int downKey) {
    this->upKey = upKey;
    this->downKey = downKey;
}

void Paddle::updatePosition() {
    if (keysPressed->contains(upKey) && y() > 0) {
        setY(y() - speed);
    }
    if (keysPressed->contains(downKey) && y() + rect().height() < screenHeight) {
        setY(y() + speed);
    }
}


void PaddleNetwork::updatePositionFromNetwork(float y) {
    setY(y);
}

Paddle::Player Paddle::getPlayerId() const {
    return playerId;
}

void Paddle::moveUp() {
    if (y() > 0) {
        setY(y() - speed);
    }
}

void Paddle::moveDown() {
    if (y() + rect().height() < screenHeight) {
        setY(y() + speed);
    }
}
