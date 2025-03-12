#ifndef COLLISION_H
#define COLLISION_H

#include <QObject>

#include "game.h"

/**
 * @brief Classe représentant la gestion des collisions dans le jeu.
 */
class Collision : public QObject {
Q_OBJECT
public:
    /**
     * @brief Constructeur de la classe Collision.
     * @param game Pointeur vers l'objet Game.
     * @param parent Le parent QObject.
     */
    explicit Collision(Game* game, QObject* parent = nullptr);

signals:
    /**
     * @brief Signal émis lorsqu'une collision avec une limite est détectée.
     */
    void boundaryHitSignal();

    /**
     * @brief Signal émis lorsqu'une collision avec une raquette est détectée.
     */
    void paddleHitSignal();

private slots:
    /**
     * @brief Slot appelé lorsqu'une collision avec une raquette est détectée.
     */
    void paddleHit();

    /**
     * @brief Slot appelé lorsqu'une collision avec une limite est détectée.
     */
    void boundaryHit();
};

#endif // COLLISION_H