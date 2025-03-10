#ifndef PLAYER_H
#define PLAYER_H

#include "paddle.h"

class Player {
public:
    Player(Paddle* paddle, int upKey, int downKey);

    Paddle* getPaddle() const;
    void setKeys(int upKey, int downKey);

private:
    Paddle* paddle;
};

#endif // PLAYER_H
