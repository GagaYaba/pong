#include "../include/score.h"
#include <QFont>
#include <QMessageBox>

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
    if (team1Score < 3 && team2Score < 3) {
        team1Score++;
        updateDisplay();
        checkGameOver();
    }
}

void Score::increaseTeam2Score() {
    if (team1Score < 3 && team2Score < 3) {
        team2Score++;
        updateDisplay();
        checkGameOver();
    }
}

void Score::updateDisplay() {
    setPlainText(QString("%1   |   %2").arg(team1Score).arg(team2Score));
}

void Score::checkGameOver() {
    if (team1Score == 3) {
        QMessageBox::information(nullptr, "Fin de partie", "TEAM ONE WIN !");
        resetScore();
    } else if (team2Score == 3) {
        QMessageBox::information(nullptr, "Fin de partie", "TEAM TWO WIN !");
        resetScore();
    }
}

void Score::resetScore() {
    team1Score = 0;
    team2Score = 0;
    updateDisplay();
}
