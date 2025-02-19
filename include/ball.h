#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include "collision.h"

class Game;

class Ball : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    Ball(int screenWidth, int screenHeight, Game* game, QObject* parent = nullptr);

public slots:
    void move();

private:
    int screenWidth, screenHeight;
    float dx, dy;
    QTimer* movementTimer;
    int direction;
    Game* game;
    Collision* collision;

    QElapsedTimer timer;
    float deltaTime;

    void handleWallCollision();
    void handlePaddleCollision();
    void handleBoundary();
};

#endif // BALL_H
