#ifndef PADDLE_H
#define PADDLE_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>

/**
 * @brief Classe représentant une raquette dans le jeu.
 */
class Paddle : public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
    /**
     * @brief Enumération des joueurs.
     */
    enum Player { p1 = 1, p2 = 2, p3 = 3, p4 = 4 };

    /**
     * @brief Constructeur de la classe Paddle.
     * @param x La position x initiale de la raquette.
     * @param y La position y initiale de la raquette.
     * @param speed La vitesse de déplacement de la raquette.
     * @param screenHeight La hauteur de l'écran.
     * @param keysPressed Ensemble des touches pressées.
     * @param playerId L'identifiant du joueur.
     * @param parent Le parent QObject.
     */
    Paddle(int x, int y, int speed, int screenHeight, QSet<int>* keysPressed, Player playerId, QObject* parent = nullptr);

    /**
     * @brief Définit les touches de déplacement de la raquette.
     * @param upKey La touche pour monter.
     * @param downKey La touche pour descendre.
     */
    void setKeys(int upKey, int downKey);

    /**
     * @brief Déplace la raquette vers le haut.
     */
    void moveUp();

    /**
     * @brief Déplace la raquette vers le bas.
     */
    void moveDown();

public slots:
    /**
     * @brief Met à jour la position de la raquette.
     */
    void updatePosition();

    /**
     * @brief Obtient l'identifiant du joueur.
     * @return L'identifiant du joueur.
     */
    Player getPlayerId() const;

signals:
    /**
     * @brief Signal émis lorsque la raquette est touchée par la balle.
     */
    void touchedByBall();

private:
    int upKey, downKey; ///< Touche pour monter et descendre.
    int speed; ///< Vitesse de déplacement de la raquette.
    int screenHeight; ///< Hauteur de l'écran.
    QTimer* movementTimer; ///< Timer pour le mouvement de la raquette.
    QSet<int>* keysPressed; ///< Ensemble des touches pressées.
    Player playerId; ///< Identifiant du joueur.
};

/**
 * @brief Classe représentant une raquette dans le réseau.
 */
class PaddleNetwork : public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
    /**
     * @brief Enumération des joueurs.
     */
    enum Player { p1 = 1, p2 = 2, p3 = 3, p4 = 4 };

    /**
     * @brief Constructeur de la classe PaddleNetwork.
     * @param x La position x initiale de la raquette.
     * @param y La position y initiale de la raquette.
     * @param speed La vitesse de déplacement de la raquette.
     * @param screenHeight La hauteur de l'écran.
     * @param playerId L'identifiant du joueur.
     * @param parent Le parent QObject.
     */
    PaddleNetwork(int x, int y, int speed, int screenHeight, Player playerId, QObject* parent = nullptr);

public slots:
    /**
     * @brief Met à jour la position de la raquette.
     * @param y La nouvelle position y de la raquette.
     */
    void updatePosition(float y);

    /**
     * @brief Met à jour la position de la raquette à partir du réseau.
     * @param y La nouvelle position y de la raquette.
     */
    void updatePositionFromNetwork(float y);

signals:
    /**
     * @brief Signal émis lorsque la raquette est touchée par la balle.
     */
    void touchedByBall();

private:
    int speed; ///< Vitesse de déplacement de la raquette.
    int screenHeight; ///< Hauteur de l'écran.
    Player playerId; ///< Identifiant du joueur.
};

#endif // PADDLE_H