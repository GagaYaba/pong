#include "../include/player.h"

Player::Player(Paddle* paddle, int upKey, int downKey) : paddle(paddle) {
    paddle->setKeys(upKey, downKey);
}

Paddle* Player::getPaddle() const {
    return paddle;
}

void Player::setKeys(int upKey, int downKey) {
    paddle->setKeys(upKey, downKey);
}
