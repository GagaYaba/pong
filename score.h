#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>

class Score : public QGraphicsTextItem {
public:
    Score(int x, int y, QGraphicsItem *parent = nullptr);

    void increasePlayer1Score();
    void increasePlayer2Score();
    void updateDisplay();

private:
    int player1Score;
    int player2Score;
};

#endif // SCORE_H
