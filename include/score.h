#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>

/**
 * @brief Classe représentant le score dans le jeu.
 */
class Score : public QGraphicsTextItem {
public:
    /**
     * @brief Constructeur de la classe Score.
     * @param x La position x initiale du score.
     * @param y La position y initiale du score.
     * @param parent Le parent QGraphicsItem.
     */
    Score(int x, int y, QGraphicsItem *parent = nullptr);

    /**
     * @brief Augmente le score de l'équipe 1.
     */
    void increaseTeam1Score();

    /**
     * @brief Augmente le score de l'équipe 2.
     */
    void increaseTeam2Score();

    /**
     * @brief Met à jour l'affichage du score.
     */
    void updateDisplay();

private:
    int team1Score; ///< Score de l'équipe 1.
    int team2Score; ///< Score de l'équipe 2.
};

#endif // SCORE_H