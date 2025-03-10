#ifndef PLAYER_H
#define PLAYER_H

#include "paddle.h"
#include <QSet>

class Player {
public:
    Player(Paddle* paddle, int upKey, int downKey, QSet<int>* keysPressed);

    void update(); // Ajout de la méthode pour bouger le paddle
    Paddle* getPaddle() const;

private:
    Paddle* paddle;
    int upKey;
    int downKey;
    QSet<int>* keysPressed; // Ajout des touches pressées
};

#endif // PLAYER_H
