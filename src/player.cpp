#include "../include/player.h"

/**
 * @brief Constructeur de la classe Player.
 * @param paddle Un pointeur vers l'objet Paddle associé.
 * @param role Le rôle du joueur.
 * @param upKey La touche pour monter.
 * @param downKey La touche pour descendre.
 * @param keysPressed Un pointeur vers l'ensemble des touches pressées.
 */
Player::Player(Paddle* paddle, QString role, int upKey, int downKey, QSet<int>* keysPressed) {
    this->paddle = paddle;
    this->type = "Player";
    this->role = role;
    this->upKey = upKey;
    this->downKey = downKey;
    this->keysPressed = keysPressed;
}

/**
 * @brief Constructeur de la classe PlayerNetwork.
 * @param paddle Un pointeur vers l'objet PaddleNetwork associé.
 * @param role Le rôle du joueur.
 */
PlayerNetwork::PlayerNetwork(PaddleNetwork* paddle, QString role) {
    this->paddle = paddle;
    this->type = "PlayerNetwork";
    this->role = role;
}

/**
 * @brief Obtient le pointeur vers l'objet Paddle associé.
 * @return Un pointeur vers l'objet Paddle.
 */
Paddle* Player::getPaddle() const {
    return paddle;
}

/**
 * @brief Obtient le pointeur vers l'objet PaddleNetwork associé.
 * @return Un pointeur vers l'objet PaddleNetwork.
 */
PaddleNetwork* PlayerNetwork::getPaddleNetwork() const {
    return paddle;
}

/**
 * @brief Met à jour la position de la raquette en fonction des touches pressées.
 */
void Player::update() {
    if (keysPressed->contains(upKey)) {
        paddle->moveUp();
    }
    if (keysPressed->contains(downKey)) {
        paddle->moveDown();
    }
}

/**
 * @brief Met à jour la position de la raquette réseau.
 * @param y La nouvelle position y.
 */
void PlayerNetwork::updatePaddlePosition(float y) {
    paddle->updatePosition(y);
}