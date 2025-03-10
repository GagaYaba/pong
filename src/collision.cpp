#include "../include/collision.h"
#include "../include/ball.h"
#include "../include/game.h"

#include <QDebug>

Collision::Collision(Game* game, QObject* parent) : QObject(parent) {
    connect(game->getBall(), &Ball::paddleHit, this, &Collision::paddleHit);
    connect(game->getBall(), &Ball::boundaryHit, this, &Collision::boundaryHit);
}

void Collision::paddleHit() {
    emit paddleHitSignal();
}

void Collision::boundaryHit() {
    emit boundaryHitSignal();
}
