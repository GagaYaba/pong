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
    void increasePlayer1Score();
    void increasePlayer2Score();
    Paddle* getPlayer1();
    Paddle* getPlayer2();
    Ball* getBall();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
private:
    QGraphicsScene* scene;
    Paddle* player1;
    Paddle* player2;
    Ball* ball;
    QSet<int>* keysPressed;
    Score* score;

signals:
    void gameClosed();
};

#endif // GAME_H
