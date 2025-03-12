#include "../include/score.h"
#include <QFont>

/**
 * @brief Constructeur de la classe Score.
 * @param x La position x initiale du score.
 * @param y La position y initiale du score.
 * @param parent Le parent QGraphicsItem.
 */
Score::Score(int x, int y, QGraphicsItem *parent)
        : QGraphicsTextItem(parent), team1Score(0), team2Score(0) {

    setDefaultTextColor(Qt::black);
    setFont(QFont("Arial", 16));

    updateDisplay();

    int textWidth = boundingRect().width();
    int screenWidth = 600;
    int xPos = (screenWidth - textWidth) / 2;

    setPos(xPos, y);
}

/**
 * @brief Augmente le score de l'équipe 1.
 */
void Score::increaseTeam1Score() {
    team1Score++;
    updateDisplay();
}

/**
 * @brief Augmente le score de l'équipe 2.
 */
void Score::increaseTeam2Score() {
    team2Score++;
    updateDisplay();
}

/**
 * @brief Met à jour l'affichage du score.
 */
void Score::updateDisplay() {
    setPlainText(QString("%1   |   %2").arg(team1Score).arg(team2Score));
}