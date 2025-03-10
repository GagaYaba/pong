#include "../include/score.h"
#include <QFont>

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

void Score::increaseTeam1Score() {
    team1Score++;
    updateDisplay();
}

void Score::increaseTeam2Score() {
    team2Score++;
    updateDisplay();
}

void Score::updateDisplay() {
    setPlainText(QString("%1   |   %2").arg(team1Score).arg(team2Score));
}
