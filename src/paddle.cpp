#include "../include/paddle.h"

#include <QKeyEvent>
#include <QDebug>

/**
 * @brief Constructeur de la classe Paddle.
 * @param x La position x initiale de la raquette.
 * @param y La position y initiale de la raquette.
 * @param speed La vitesse de déplacement de la raquette.
 * @param screenHeight La hauteur de l'écran.
 * @param keysPressed Un pointeur vers l'ensemble des touches pressées.
 * @param playerId L'identifiant du joueur.
 * @param parent Le parent QObject.
 */
Paddle::Paddle(int x, int y, int speed, int screenHeight, QSet<int>* keysPressed, Player playerId, QObject* parent)
        : QObject(parent), speed(speed), screenHeight(screenHeight), keysPressed(keysPressed), playerId(playerId) {
    setRect(0, 0, 10, 60);
    qDebug() << x << " P "<< y;
    setPos(x, y);

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Paddle::updatePosition);
    movementTimer->start(16);
}

/**
 * @brief Constructeur de la classe PaddleNetwork.
 * @param x La position x initiale de la raquette réseau.
 * @param y La position y initiale de la raquette réseau.
 * @param speed La vitesse de déplacement de la raquette réseau.
 * @param screenHeight La hauteur de l'écran.
 * @param playerId L'identifiant du joueur.
 * @param parent Le parent QObject.
 */
PaddleNetwork::PaddleNetwork(int x, int y, int speed, int screenHeight, Player playerId, QObject* parent)
        : QObject(parent), speed(speed), screenHeight(screenHeight), playerId(playerId) {
    qDebug() << x << "  "<< y;
    qDebug() << "PaddleNetwork created";
    setRect(0, 0, 10, 60);
    setPos(x, y);
}

/**
 * @brief Met à jour la position de la raquette réseau.
 * @param y La nouvelle position y.
 */
void PaddleNetwork::updatePosition(float y) {
    setY(y);
}

/**
 * @brief Définit les touches de déplacement de la raquette.
 * @param upKey La touche pour monter.
 * @param downKey La touche pour descendre.
 */
void Paddle::setKeys(int upKey, int downKey) {
    this->upKey = upKey;
    this->downKey = downKey;
}

/**
 * @brief Met à jour la position de la raquette en fonction des touches pressées.
 */
void Paddle::updatePosition() {
    if (keysPressed->contains(upKey) && y() > 0) {
        setY(y() - speed);
    }
    if (keysPressed->contains(downKey) && y() + rect().height() < screenHeight) {
        setY(y() + speed);
    }
}

/**
 * @brief Met à jour la position de la raquette réseau à partir des données réseau.
 * @param y La nouvelle position y.
 */
void PaddleNetwork::updatePositionFromNetwork(float y) {
    setY(y);
}

/**
 * @brief Obtient l'identifiant du joueur.
 * @return L'identifiant du joueur.
 */
Paddle::Player Paddle::getPlayerId() const {
    return playerId;
}

/**
 * @brief Déplace la raquette vers le haut.
 */
void Paddle::moveUp() {
    if (y() > 0) {
        setY(y() - speed);
    }
}

/**
 * @brief Déplace la raquette vers le bas.
 */
void Paddle::moveDown() {
    if (y() + rect().height() < screenHeight) {
        setY(y() + speed);
    }
}