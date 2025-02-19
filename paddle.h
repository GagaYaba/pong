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
    Paddle(int x, int y, int speed, int screenHeight, QSet<int>* keysPressed, QObject* parent = nullptr);

    void setKeys(int upKey, int downKey);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void updatePosition();

private:
    int upKey, downKey;
    int speed;
    int screenHeight;
    QTimer* movementTimer;
    QSet<int>* keysPressed;
};

#endif // PADDLE_H
