#include "../include/player.h"

Player::Player(Paddle* paddle, int upKey, int downKey, QSet<int>* keysPressed) {
    this->paddle = paddle;
    this->upKey = upKey;
    this->downKey = downKey;
    this->keysPressed = keysPressed;
}

Paddle* Player::getPaddle() const {
    return paddle;
}

void Player::update() {
    if (keysPressed->contains(upKey)) {
        paddle->moveUp();
    }
    if (keysPressed->contains(downKey)) {
        paddle->moveDown();
    }
}
