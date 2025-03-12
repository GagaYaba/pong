#include "../include/ball.h"
#include "../include/boundary.h"
#include "../include/paddle.h"
#include "../include/game.h"
#include "../include/globals.h"

#include <QGraphicsScene>
#include <QList>
#include <QDebug>

/**
 * @brief Constructeur de la classe Ball.
 * @param screenWidth La largeur de l'écran.
 * @param screenHeight La hauteur de l'écran.
 * @param game Pointeur vers l'objet Game.
 * @param parent Le parent QObject.
 */
Ball::Ball(int screenWidth, int screenHeight, Game* game, QObject* parent)
        : QObject(parent), screenWidth(screenWidth), screenHeight(screenHeight), game(game) {

    setRect(0, 0, 10, 10);
    setPos(screenWidth / 2, screenHeight / 2);

    dx = 2;
    dy = 0;
    direction = -1;

    if (g_isHost) {
        movementTimer = new QTimer(this);
        connect(movementTimer, &QTimer::timeout, this, &Ball::move);
        movementTimer->start(16);
        timer.start();
    }
}

/**
 * @brief Déplace la balle.
 */
void Ball::move() {
    deltaTime = timer.elapsed() / 16.0f;
    timer.restart();

    setX(x() + dx * direction * deltaTime);
    setY(y() + dy * deltaTime);

    handleCollisions();
}

/**
 * @brief Définit la position de la balle.
 * @param x La position x.
 * @param y La position y.
 */
void Ball::setPos(int x, int y) {
    setX(x);
    setY(y);
}

/**
 * @brief Gère les collisions de la balle.
 */
void Ball::handleCollisions() {
    handlePaddleCollision();
    handleBoundaryCollision();
}

/**
 * @brief Gère les collisions de la balle avec les raquettes.
 */
void Ball::handlePaddleCollision() {
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        Paddle* paddle = dynamic_cast<Paddle*>(item);
        PaddleNetwork* paddleNetwork = dynamic_cast<PaddleNetwork*>(item);
        if (paddle || paddleNetwork) {
            handlePaddleHit(paddle, paddleNetwork);
            reverseXDirection();
            adjustVerticalSpeed(paddle, paddleNetwork);

            adjustSpeed();
            limitMaxSpeed();
            qDebug() << "Ball hit the paddle!";
            emit paddleHit();
        }
    }
}

/**
 * @brief Gère les collisions de la balle avec les limites de l'écran.
 */
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

/**
 * @brief Traite un but marqué.
 */
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

/**
 * @brief Réinitialise la position de la balle.
 */
void Ball::resetBall() {
    setPos(screenWidth / 2, screenHeight / 2);
    direction = -direction;
    dx = 2;
    dy = 0;
    hit = false;
}

/**
 * @brief Gère le cas où la balle touche une raquette.
 * @param paddle Pointeur vers la raquette.
 * @param paddleNetwork Pointeur vers la raquette du réseau.
 */
void Ball::handlePaddleHit(Paddle* paddle, PaddleNetwork* paddleNetwork) {
    if (!hit) {
        hit = true;
    }

    if (g_playerRole == "p1") {
        if (paddle == game->getPaddle(0)) {
            lastTouchedByPlayer = P1;
        } else if (paddleNetwork == game->getPaddleNetwork(0)) {
            lastTouchedByPlayer =  P2;
        }
    } else if (g_playerRole == "p2") {
        if (paddle == game->getPaddle(0)) {
            lastTouchedByPlayer = P2;
        } else if (paddleNetwork == game->getPaddleNetwork(0)) {
            lastTouchedByPlayer = P1;
        }
    }
}

/**
 * @brief Inverse la direction horizontale de la balle.
 */
void Ball::reverseXDirection() {
    dx = -dx;
}

/**
 * @brief Ajuste la vitesse verticale de la balle en fonction de la position de la raquette.
 * @param paddle Pointeur vers la raquette.
 * @param paddleNetwork Pointeur vers la raquette du réseau.
 */
void Ball::adjustVerticalSpeed(Paddle* paddle, PaddleNetwork* paddleNetwork) {
    if (paddle) {
        float hitPosition = (y() + rect().height() / 2) - (paddle->y() + paddle->rect().height() / 2);
        dy += hitPosition * 0.1;
    } else if (paddleNetwork) {
        float hitPosition = (y() + rect().height() / 2) - (paddleNetwork->y() + paddleNetwork->rect().height() / 2);
        dy += hitPosition * 0.1;
    }
}

/**
 * @brief Limite la vitesse maximale de la balle.
 */
void Ball::limitMaxSpeed() {
    float maxSpeed = 6.0f;
    if (std::abs(dx) > maxSpeed) {
        dx = (dx > 0 ? 1 : -1) * maxSpeed;
    }
    if (std::abs(dy) > maxSpeed) {
        dy = (dy > 0 ? 1 : -1) * maxSpeed;
    }
}