#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <QObject>

class Boundary : public QObject {
    Q_OBJECT
public:
    enum Player { P1, P2 };

    Boundary(Player playerId, int xPos, int width, QObject* parent = nullptr)
        : QObject(parent), playerId(playerId), xPos(xPos), width(width) {}

    Player getPlayerId() const { return playerId; }
    int getXPos() const { return xPos; }
    int getWidth() const { return width; }

private:
    Player playerId;
    int xPos;
    int width;
};

#endif // BOUNDARY_H

