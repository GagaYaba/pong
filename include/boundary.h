#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <QObject>

/**
 * @brief Classe représentant une limite dans le jeu.
 */
class Boundary : public QObject {
Q_OBJECT
public:
    /**
     * @brief Enumération des côtés de la limite.
     */
    enum Side {
        Left, ///< Côté gauche.
        Right, ///< Côté droit.
        Top, ///< Côté supérieur.
        Bottom ///< Côté inférieur.
    };

    /**
     * @brief Constructeur de la classe Boundary.
     * @param side Le côté de la limite.
     * @param parent Le parent QObject.
     */
    Boundary(Side side, QObject* parent = nullptr);

    /**
     * @brief Obtient le côté de la limite.
     * @return Le côté de la limite.
     */
    Side side() const;

    /**
     * @brief Définit l'équipe associée à la limite.
     * @param teamId L'identifiant de l'équipe.
     */
    void setTeam(int teamId);

    /**
     * @brief Obtient l'équipe associée à la limite.
     * @return L'identifiant de l'équipe.
     */
    int team() const;

signals:
    /**
     * @brief Signal émis lorsque la balle touche la limite.
     * @param side Le côté de la limite touché.
     */
    void touchedByBall(Boundary::Side side);

private:
    Side m_side; ///< Le côté de la limite.
    int m_team; ///< L'identifiant de l'équipe associée à la limite.
};

#endif // BOUNDARY_H