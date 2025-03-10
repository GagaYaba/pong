#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <QObject>

class Boundary : public QObject {
    Q_OBJECT
public:
    enum Side {
        Left, Right, Top, Bottom
    };

    Boundary(Side side, QObject* parent = nullptr);

    Side side() const;
    void setTeam(int teamId);
    int team() const;

signals:
    void touchedByBall(Boundary::Side side);

private:
    Side m_side;
    int m_team;
};

#endif // BOUNDARY_H
