#include "../include/score.h"
#include <QFont>

Score::Score(int x, int y, QGraphicsItem *parent)
    : QGraphicsTextItem(parent), player1Score(0), player2Score(0) {

    setDefaultTextColor(Qt::black);
    setFont(QFont("Arial", 16));
    setPos(x, y);

    updateDisplay();
}

void Score::increasePlayer1Score() {
    player1Score++;
    updateDisplay();
}

void Score::increasePlayer2Score() {
    player2Score++;
    updateDisplay();
}

void Score::updateDisplay() {
    setPlainText(QString("%1  |  %2").arg(player1Score).arg(player2Score));
}
