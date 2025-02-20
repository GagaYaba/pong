#include "../include/boundary.h"

Boundary::Boundary(Side side, QObject* parent)
    : QObject(parent), m_side(side), m_team(0) {}

Boundary::Side Boundary::side() const {
    return m_side;
}

void Boundary::setTeam(int teamId) {
    m_team = teamId;
}

int Boundary::team() const {
    return m_team;
}
