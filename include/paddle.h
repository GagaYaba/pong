#ifndef PADDLE_H
#define PADDLE_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>

class Paddle : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    enum Player { p1 = 1, p2 = 2, p3 = 3, p4 = 4 };

    Paddle(int x, int y, int speed, int screenHeight, QSet<int>* keysPressed, Player playerId, QObject* parent = nullptr);

    void setKeys(int upKey, int downKey);

    void moveUp();
    void moveDown();
public slots:
    void updatePosition();
    Player getPlayerId() const;

signals:
    void touchedByBall();

private:
    int upKey, downKey;
    int speed;
    int screenHeight;
    QTimer* movementTimer;
    QSet<int>* keysPressed;
    Player playerId;
};

#endif // PADDLE_H
