#ifndef COLLISION_H
#define COLLISION_H

#include <QObject>

#include "game.h"

class Collision : public QObject {
    Q_OBJECT
public:
    explicit Collision(Game* game, QObject* parent = nullptr);

signals:
    void boundaryHitSignal();
    void paddleHitSignal();

private slots:
    void paddleHit();
    void boundaryHit();
};

#endif // COLLISION_H
