#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QKeyEvent>
#include "paddle.h"
#include "ball.h"
#include "score.h"
#include "player.h"

class Game : public QGraphicsView {
    Q_OBJECT
public:
    enum GameMode { OneVOne, TwoVTwo };

    explicit Game(QWidget *parent = nullptr, GameMode mode = OneVOne);

    void increaseTeam1Score();
    void increaseTeam2Score();
    Paddle* getPaddle(int playerIndex);
    Ball* getBall();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void setupPlayersAndPaddles();
    GameMode gameMode;

    QGraphicsScene* scene;
    QList<Player*> players;
    Ball* ball;
    QSet<int>* keysPressed;
    Score* score;
};

#endif // GAME_H
