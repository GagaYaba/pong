#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

#include "paddle.h"

class Game;

class Ball : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    Ball(int screenWidth, int screenHeight, Game* game, QObject* parent = nullptr);
    void setPos(int x, int y);

public slots:
    void move();

signals:
    void paddleHit();
    void boundaryHit();

private:
    int screenWidth, screenHeight;
    float dx, dy;
    QTimer* movementTimer;
    int direction;
    Game* game;
    QElapsedTimer timer;
    float deltaTime;

    enum LastTouchedBy { P1, P2, P3, P4, None };
    LastTouchedBy lastTouchedByPlayer;

    void handleCollisions();
    void handlePaddleCollision();
    void handleBoundaryCollision();
    void processGoal();
    void resetBall();
    void adjustSpeed();
    void limitMaxSpeed();
    void handlePaddleHit(Paddle* paddle, PaddleNetwork* paddleNetwork);
    void reverseXDirection();
    void adjustVerticalSpeed(Paddle* paddle, PaddleNetwork* paddleNetwork);

    bool hit = false;
};

#endif // BALL_H
