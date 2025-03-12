#include "../include/boundary.h"

/**
 * @brief Constructeur de la classe Boundary.
 * @param side Le côté de la limite.
 * @param parent Le parent QObject.
 */
Boundary::Boundary(Side side, QObject* parent)
        : QObject(parent), m_side(side), m_team(0) {}

/**
 * @brief Obtient le côté de la limite.
 * @return Le côté de la limite.
 */
Boundary::Side Boundary::side() const {
    return m_side;
}

/**
 * @brief Définit l'équipe associée à la limite.
 * @param teamId L'identifiant de l'équipe.
 */
void Boundary::setTeam(int teamId) {
    m_team = teamId;
}

/**
 * @brief Obtient l'équipe associée à la limite.
 * @return L'identifiant de l'équipe.
 */
int Boundary::team() const {
    return m_team;
}