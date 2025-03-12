#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include "paddle.h"

class Game;

/**
 * @brief Classe représentant la balle dans le jeu.
 */
class Ball : public QObject, public QGraphicsEllipseItem {
Q_OBJECT
public:
    /**
     * @brief Constructeur de la classe Ball.
     * @param screenWidth La largeur de l'écran.
     * @param screenHeight La hauteur de l'écran.
     * @param game Pointeur vers l'objet Game.
     * @param parent Le parent QObject.
     */
    Ball(int screenWidth, int screenHeight, Game* game, QObject* parent = nullptr);

    /**
     * @brief Définit la position de la balle.
     * @param x La position X.
     * @param y La position Y.
     */
    void setPos(int x, int y);

public slots:
    /**
     * @brief Déplace la balle.
     */
    void move();

signals:
    /**
     * @brief Signal émis lorsque la balle touche une raquette.
     */
    void paddleHit();

    /**
     * @brief Signal émis lorsque la balle touche une limite.
     */
    void boundaryHit();

private:
    int screenWidth, screenHeight; ///< Largeur et hauteur de l'écran.
    float dx, dy; ///< Vitesse de déplacement de la balle en X et Y.
    QTimer* movementTimer; ///< Timer pour gérer le mouvement de la balle.
    int direction; ///< Direction de la balle.
    Game* game; ///< Pointeur vers l'objet Game.
    QElapsedTimer timer; ///< Timer pour mesurer le temps écoulé.
    float deltaTime; ///< Temps écoulé depuis le dernier mouvement.

    /**
     * @brief Enumération des joueurs ayant touché la balle en dernier.
     */
    enum LastTouchedBy { P1, P2, P3, P4, None };
    LastTouchedBy lastTouchedByPlayer; ///< Dernier joueur ayant touché la balle.

    /**
     * @brief Gère les collisions de la balle.
     */
    void handleCollisions();

    /**
     * @brief Gère les collisions avec les raquettes.
     */
    void handlePaddleCollision();

    /**
     * @brief Gère les collisions avec les limites.
     */
    void handleBoundaryCollision();

    /**
     * @brief Traite un but.
     */
    void processGoal();

    /**
     * @brief Réinitialise la position de la balle.
     */
    void resetBall();

    /**
     * @brief Ajuste la vitesse de la balle.
     */
    void adjustSpeed();

    /**
     * @brief Limite la vitesse maximale de la balle.
     */
    void limitMaxSpeed();

    /**
     * @brief Gère le cas où la balle touche une raquette.
     * @param paddle Pointeur vers la raquette.
     * @param paddleNetwork Pointeur vers le réseau de raquettes.
     */
    void handlePaddleHit(Paddle* paddle, PaddleNetwork* paddleNetwork);

    /**
     * @brief Inverse la direction en X de la balle.
     */
    void reverseXDirection();

    /**
     * @brief Ajuste la vitesse verticale de la balle en fonction de la raquette.
     * @param paddle Pointeur vers la raquette.
     * @param paddleNetwork Pointeur vers le réseau de raquettes.
     */
    void adjustVerticalSpeed(Paddle* paddle, PaddleNetwork* paddleNetwork);

    bool hit = false; ///< Indique si la balle a touché quelque chose.
};

#endif // BALL_H