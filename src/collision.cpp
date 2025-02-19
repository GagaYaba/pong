#include "../include/Collision.h"
#include "../include/paddle.h"

Collision::Collision(QObject *parent) : QObject(parent) {}

void Collision::checkCollision(QGraphicsItem* ball, const QList<QGraphicsItem*>& collidingItems, int screenHeight) {
    for (QGraphicsItem* item : collidingItems) {
        Paddle* paddle = dynamic_cast<Paddle*>(item);
        if (paddle) {
            emit paddleHit(paddle);
            return;
        }
    }

    if (ball->y() <= 0 || ball->y() + ball->boundingRect().height() >= screenHeight) {
        emit boundaryHit();
    }
}
