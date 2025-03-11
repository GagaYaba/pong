#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>

class Score : public QGraphicsTextItem {
public:
    Score(int x, int y, QGraphicsItem *parent = nullptr);

    void increaseTeam1Score();
    void increaseTeam2Score();
    void updateDisplay();
    void resetScore();  // Nouvelle méthode pour réinitialiser le score

private:
    void checkGameOver();

    int team1Score;
    int team2Score;
};

#endif // SCORE_H
