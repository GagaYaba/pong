#include "../include/paddle.h"

Paddle::Paddle(int x, int y, int speed, int screenHeight, QSet<int>* keysPressed, QObject* parent)
    : QObject(parent), speed(speed), screenHeight(screenHeight), keysPressed(keysPressed) {
    setRect(0, 0, 10, 60);
    setPos(x, y);

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Paddle::updatePosition);
    movementTimer->start(16);
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

void Paddle::keyPressEvent(QKeyEvent *event) {
    keysPressed->insert(event->key());
}

void Paddle::keyReleaseEvent(QKeyEvent *event) {
    keysPressed->remove(event->key());
}
