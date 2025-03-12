// game.cpp
#include <QMessageBox>
#include "../include/game.h"
#include "../include/globals.h"
#include "../client/GameClient/gameclient.h"

/**
 * @brief Constructeur de la classe Game.
 * @param parent Le parent QWidget.
 * @param mode Le mode de jeu.
 */
Game::Game(QWidget *parent, GameMode mode)
        : QGraphicsView(parent), gameMode(mode) {
    int screenWidth = 600;
    int screenHeight = 600;

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, screenWidth, screenHeight);
    setScene(scene);

    keysPressed = new QSet<int>();

    setupPlayersAndPaddles();

    ball = new Ball(screenWidth, screenHeight, this);
    scene->addItem(ball);

    score = new Score(120, 10);
    scene->addItem(score);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Game::updateGame);
    timer->start(16);
    setFixedSize(screenWidth, screenHeight);
    setFocus();
}

/**
 * @brief Configure les joueurs et les raquettes.
 */
void Game::setupPlayersAndPaddles() {
    int screenWidth = 600;
    int screenHeight = 600;

    if (g_playerRole == "p1") {
        players.append(new Player(
                new Paddle(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::p1), g_playerRole,
                Qt::Key_S, Qt::Key_W, keysPressed));
        playersNetwork.append(new PlayerNetwork(
                new PaddleNetwork(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, PaddleNetwork::p2), "p2"));
    } else if (g_playerRole == "p2") {
        players.append(new Player(
                new Paddle(screenWidth * 0.95 - 10, screenHeight * 0.25, 5, screenHeight, keysPressed, Paddle::p2), g_playerRole,
                Qt::Key_Up, Qt::Key_Down, keysPressed));
        playersNetwork.append(new PlayerNetwork(
                new PaddleNetwork(screenWidth * 0.05, screenHeight * 0.25, 5, screenHeight, PaddleNetwork::p1), "p1"));
        //    } else if (g_playerRole == "p3" && g_gameMode == "TwoVTwo") {
        //        players.append(new Player(new Paddle(screenWidth * 0.2, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::p3), Qt::Key_R, Qt::Key_D, keysPressed));
        //    } else if (g_playerRole == "p4" && g_gameMode == "TwoVTwo") {
        //        players.append(new Player(new Paddle(screenWidth * 0.8 - 10, screenHeight * 0.15, 5, screenHeight, keysPressed, Paddle::p4), Qt::Key_Left, Qt::Key_Right, keysPressed));
        //    }
    }

    if (!players.isEmpty()) {
        for (Player *player : players) {
            scene->addItem(player->getPaddle());
        }
    }

    if (!playersNetwork.isEmpty()) {
        for (PlayerNetwork *playerNetwork : playersNetwork) {
            scene->addItem(playerNetwork->getPaddleNetwork());
        }
    }
}

/**
 * @brief Obtient la balle du jeu.
 * @return Un pointeur vers l'objet Ball.
 */
Ball *Game::getBall() {
    return ball;
}

/**
 * @brief Augmente le score de l'équipe 1.
 */
void Game::increaseTeam1Score() {
    score->increaseTeam1Score();
}

/**
 * @brief Augmente le score de l'équipe 2.
 */
void Game::increaseTeam2Score() {
    score->increaseTeam2Score();
}

/**
 * @brief Gère l'événement de pression d'une touche.
 * @param event L'événement de pression de touche.
 */
void Game::keyPressEvent(QKeyEvent *event) {
    keysPressed->insert(event->key());
}

/**
 * @brief Gère l'événement de relâchement d'une touche.
 * @param event L'événement de relâchement de touche.
 */
void Game::keyReleaseEvent(QKeyEvent *event) {
    keysPressed->remove(event->key());
}

/**
 * @brief Obtient la raquette d'un joueur.
 * @param playerIndex L'index du joueur.
 * @return Un pointeur vers l'objet Paddle.
 */
Paddle *Game::getPaddle(int playerIndex) {
    if (playerIndex >= 0 && playerIndex < players.size()) {
        return players[playerIndex]->getPaddle();
    }
    return nullptr;
}

/**
 * @brief Obtient la raquette réseau d'un joueur.
 * @param playerIndex L'index du joueur.
 * @return Un pointeur vers l'objet PaddleNetwork.
 */
PaddleNetwork *Game::getPaddleNetwork(int playerIndex) {
    if (playerIndex >= 0 && playerIndex < playersNetwork.size()) {
        return playersNetwork[playerIndex]->getPaddleNetwork();
    }
    return nullptr;
}

/**
 * @brief Gère l'événement de fermeture de la fenêtre.
 * @param event L'événement de fermeture.
 */
void Game::closeEvent(QCloseEvent *event) {
    int ret = QMessageBox::question(this, "Quitter", "Voulez-vous vraiment quitter ?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

/**
 * @brief Met à jour le jeu.
 */
void Game::updateGame() {
    for (Player *player : players) {
        if (player->type == "Player") {
            player->update();
        }
    }
}