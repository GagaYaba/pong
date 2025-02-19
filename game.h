#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QKeyEvent>
#include "Paddle.h"
#include "Ball.h"
#include "Score.h"

class Game : public QGraphicsView {
    Q_OBJECT
public:
    Game(QWidget *parent = nullptr);
    void increasePlayer1Score();
    void increasePlayer2Score();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QGraphicsScene* scene;
    Paddle* player1;
    Paddle* player2;
    Ball* ball;
    QSet<int>* keysPressed;
    Score* score;
};

#endif // GAME_H
