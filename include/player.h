#ifndef PLAYER_H
#define PLAYER_H

#include "paddle.h"
#include <QSet>

class Player {
public:
    Player(Paddle* paddle, QString role, int upKey, int downKey, QSet<int>* keysPressed);

    virtual ~Player() = default;
    virtual Paddle* getPaddle() const; // Retourne un Paddle*
    virtual void update();

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
    PlayerNetwork(PaddleNetwork* paddle, QString role);
    PaddleNetwork* getPaddle() const; // Retourne un PaddleNetwork* correctement
    void updatePaddlePosition(float y);
    QString type;
    QString role;

private:
    PaddleNetwork* paddle;
};

#endif // PLAYER_H
