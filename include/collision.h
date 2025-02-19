#ifndef COLLISION_H
#define COLLISION_H

#include <QObject>
#include <QGraphicsItem>

class Collision : public QObject {
    Q_OBJECT
public:
    explicit Collision(QObject *parent = nullptr);

signals:
    void paddleHit(QGraphicsItem* paddle); // Signal émis quand la balle touche un paddle
    void boundaryHit(); // Signal émis quand la balle touche une boundary

public slots:
    void checkCollision(QGraphicsItem* ball, const QList<QGraphicsItem*>& collidingItems, int screenHeight);
};

#endif // COLLISION_H