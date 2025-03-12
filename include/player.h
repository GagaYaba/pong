#ifndef PLAYER_H
#define PLAYER_H

#include "paddle.h"
#include <QSet>

/**
 * @brief Classe représentant un joueur.
 */
class Player {
public:
    /**
     * @brief Constructeur de la classe Player.
     * @param paddle Pointeur vers la raquette du joueur.
     * @param role Rôle du joueur.
     * @param upKey Touche pour monter.
     * @param downKey Touche pour descendre.
     * @param keysPressed Ensemble des touches pressées.
     */
    Player(Paddle* paddle, QString role, int upKey, int downKey, QSet<int>* keysPressed);

    /**
     * @brief Destructeur virtuel de la classe Player.
     */
    virtual ~Player() = default;

    /**
     * @brief Obtient la raquette du joueur.
     * @return Pointeur vers la raquette du joueur.
     */
    virtual Paddle* getPaddle() const;

    /**
     * @brief Met à jour l'état du joueur.
     */
    virtual void update();

    QString type; ///< Type du joueur.
    QString role; ///< Rôle du joueur.

private:
    Paddle* paddle; ///< Pointeur vers la raquette du joueur.

    int upKey; ///< Touche pour monter.
    int downKey; ///< Touche pour descendre.
    QSet<int>* keysPressed; ///< Ensemble des touches pressées.
};

/**
 * @brief Classe représentant un joueur dans le réseau.
 */
class PlayerNetwork {
public:
    /**
     * @brief Constructeur de la classe PlayerNetwork.
     * @param paddle Pointeur vers la raquette du joueur dans le réseau.
     * @param role Rôle du joueur.
     */
    PlayerNetwork(PaddleNetwork* paddle, QString role);

    /**
     * @brief Obtient la raquette du joueur dans le réseau.
     * @return Pointeur vers la raquette du joueur dans le réseau.
     */
    virtual PaddleNetwork* getPaddleNetwork() const;

    /**
     * @brief Met à jour la position de la raquette du joueur dans le réseau.
     * @param y Nouvelle position y de la raquette.
     */
    void updatePaddlePosition(float y);

    QString type; ///< Type du joueur.
    QString role; ///< Rôle du joueur.

private:
    PaddleNetwork* paddle; ///< Pointeur vers la raquette du joueur dans le réseau.
};

#endif // PLAYER_H