#ifndef COLLISION_H
#define COLLISION_H

#include <QObject>

class Collision : public QObject {
    Q_OBJECT
public:
    explicit Collision(QObject* parent = nullptr);

signals:
    void boundaryHit();
    void paddleHit();
};

#endif // COLLISION_H
