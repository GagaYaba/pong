// game.h
#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QKeyEvent>
#include "paddle.h"
#include "ball.h"
#include "score.h"
#include "player.h"

/**
 * @brief Classe représentant le jeu.
 */
class Game : public QGraphicsView {
Q_OBJECT
public:
    /**
     * @brief Enumération des modes de jeu.
     */
    enum GameMode { OneVOne, TwoVTwo };

    /**
     * @brief Constructeur de la classe Game.
     * @param parent Le parent QWidget.
     * @param mode Le mode de jeu.
     */
    Game(QWidget *parent = nullptr, GameMode mode = OneVOne);

    /**
     * @brief Augmente le score de l'équipe 1.
     */
    void increaseTeam1Score();

    /**
     * @brief Augmente le score de l'équipe 2.
     */
    void increaseTeam2Score();

    /**
     * @brief Obtient la raquette d'un joueur.
     * @param playerIndex L'index du joueur.
     * @return Un pointeur vers la raquette.
     */
    Paddle* getPaddle(int playerIndex);

    /**
     * @brief Obtient le réseau de raquettes d'un joueur.
     * @param playerIndex L'index du joueur.
     * @return Un pointeur vers le réseau de raquettes.
     */
    PaddleNetwork* getPaddleNetwork(int playerIndex);

    /**
     * @brief Obtient la balle.
     * @return Un pointeur vers la balle.
     */
    Ball* getBall();

    /**
     * @brief Met à jour le jeu.
     */
    void updateGame();

    QList<Player*> players; ///< Liste des joueurs.
    QList<PlayerNetwork*> playersNetwork; ///< Liste des réseaux de joueurs.

protected:
    /**
     * @brief Gère l'événement de pression d'une touche.
     * @param event L'événement de pression de touche.
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Gère l'événement de relâchement d'une touche.
     * @param event L'événement de relâchement de touche.
     */
    void keyReleaseEvent(QKeyEvent *event) override;

    /**
     * @brief Gère l'événement de fermeture de la fenêtre.
     * @param event L'événement de fermeture.
     */
    void closeEvent(QCloseEvent *event) override;

private:
    /**
     * @brief Configure les joueurs et les raquettes.
     */
    void setupPlayersAndPaddles();

    GameMode gameMode; ///< Le mode de jeu.
    QGraphicsScene* scene; ///< La scène graphique.
    Ball* ball; ///< La balle.
    QSet<int>* keysPressed; ///< Ensemble des touches pressées.
    Score* score; ///< Le score.

signals:
    /**
     * @brief Signal émis lorsque le jeu est fermé.
     */
    void gameClosed();
};

#endif // GAME_H