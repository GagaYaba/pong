// game.h
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

    Game(QWidget *parent = nullptr, GameMode mode = OneVOne);
    void increaseTeam1Score();
    void increaseTeam2Score();
    Paddle* getPaddle(int playerIndex);
    PaddleNetwork* getPaddleNetwork(int playerIndex);
    Ball* getBall();
    void updateGame();
    QList<Player*> players;
    QList<PlayerNetwork*> playersNetwork;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
private:
    void setupPlayersAndPaddles();
    GameMode gameMode;
    QGraphicsScene* scene;
    Ball* ball;
    QSet<int>* keysPressed;
    Score* score;

signals:
    void gameClosed();
};

#endif // GAME_H
