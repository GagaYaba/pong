// #include "../include/player.h"

// Player::Player(Paddle* paddle, QString role, int upKey, int downKey, QSet<int>* keysPressed) {
//     this->paddle = paddle;
//     this->type = "Player";
//     this->role = role;
//     this->upKey = upKey;
//     this->downKey = downKey;
//     this->keysPressed = keysPressed;
// }

// Player::PlayerNetwork(Paddle* paddle, QString role) {
//     this->paddle = paddle;
//     this->type = "PlayerNetwork";
//     this->role = role;
// }

// Paddle* Player::getPaddle() const {
//     return paddle;
// }

// void Player::update() {
//     if (keysPressed->contains(upKey)) {
//         paddle->moveUp();
//     }
//     if (keysPressed->contains(downKey)) {
//         paddle->moveDown();
//     }
// }

#include "../include/player.h"

Player::Player(Paddle* paddle, QString role, int upKey, int downKey, QSet<int>* keysPressed) {
    this->paddle = paddle;
    this->type = "Player";
    this->role = role;
    this->upKey = upKey;
    this->downKey = downKey;
    this->keysPressed = keysPressed;
}

PlayerNetwork::PlayerNetwork(PaddleNetwork* paddle, QString role) {
    this->paddle = paddle;
    this->type = "PlayerNetwork";
    this->role = role;
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
