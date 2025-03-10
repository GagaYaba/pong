#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QKeyEvent>
#include "paddle.h"
#include "ball.h"
#include "score.h"

class Game : public QGraphicsView {
    Q_OBJECT
public:
    Game(QWidget *parent = nullptr);

    void increaseTeam1Score();
    void increaseTeam2Score();
    Paddle* getPaddle(int playerIndex);
    Ball* getBall();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QGraphicsScene* scene;
    Paddle* paddles[4];
    Ball* ball;
    QSet<int>* keysPressed;
    Score* score;
};

#endif // GAME_H
