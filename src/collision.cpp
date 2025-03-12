#include "../include/collision.h"
#include "../include/ball.h"
#include "../include/game.h"

#include <QDebug>

/**
 * @brief Constructeur de la classe Collision.
 * @param game Pointeur vers l'objet Game.
 * @param parent Le parent QObject.
 */
Collision::Collision(Game* game, QObject* parent) : QObject(parent) {
    connect(game->getBall(), &Ball::paddleHit, this, &Collision::paddleHit);
    connect(game->getBall(), &Ball::boundaryHit, this, &Collision::boundaryHit);
}

/**
 * @brief Slot appelé lorsque la balle touche une raquette.
 * Émet le signal paddleHitSignal.
 */
void Collision::paddleHit() {
    emit paddleHitSignal();
}

/**
 * @brief Slot appelé lorsque la balle touche une limite.
 * Émet le signal boundaryHitSignal.
 */
void Collision::boundaryHit() {
    emit boundaryHitSignal();
}