#include "../include/score.h"
#include <QFont>

Score::Score(int x, int y, QGraphicsItem *parent)
    : QGraphicsTextItem(parent), player1Score(0), player2Score(0) {

    setDefaultTextColor(Qt::black);
    setFont(QFont("Arial", 16));

    updateDisplay();

    int textWidth = boundingRect().width();
    int screenWidth = 400;
    int xPos = (screenWidth - textWidth) / 2;

    setPos(xPos, y);
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
    setPlainText(QString("%1   |   %2").arg(player1Score).arg(player2Score));
}
