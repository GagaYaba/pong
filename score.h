#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QFont>

class Score : public QGraphicsTextItem {
public:
    Score(int x, int y, QGraphicsItem *parent = nullptr);

    void increasePlayer1Score();
    void increasePlayer2Score();

private:
    int player1Score;
    int player2Score;

    void updateDisplay();
};

#endif // SCORE_H
