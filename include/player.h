#ifndef PLAYER_H
#define PLAYER_H

#include "paddle.h"
#include <QSet>

class Player {
public:
    Player(Paddle* paddle, QString role, int upKey, int downKey, QSet<int>* keysPressed);

    void update(); // Ajout de la méthode pour bouger le paddle
    Paddle* getPaddle() const;
    QString type;
    QString role;

private:
    Paddle* paddle;

    int upKey;
    int downKey;
    QSet<int>* keysPressed; // Ajout des touches pressées
};

class PlayerNetwork {
public:
    explicit PlayerNetwork(PaddleNetwork* paddle, QString role);
    QString type;
    QString role;

private:
    PaddleNetwork* paddle;
};

#endif // PLAYER_H