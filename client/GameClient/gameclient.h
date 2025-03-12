#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <memory>
#include <vector>

#include "ClientEventHandler.h"
#include "ClientEventHandlerFactory.h"
#include "BinaryClientEventHandler.h"
#include "BinaryEventHandlerFactory.h"

/**
 * @brief Classe représentant le client de jeu.
 */
class GameClient : public QObject
{
Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe GameClient.
     * @param parent Le parent QObject.
     */
    explicit GameClient(QObject *parent = nullptr);

    /**
     * @brief Se connecte au serveur.
     * @param serverAddr L'adresse du serveur.
     */
    void connectToServer(const QHostAddress &serverAddr);

    /**
     * @brief Démarre la partie.
     */
    void startGame();

    /**
     * @brief Envoie un message au serveur.
     * @param message Le message à envoyer.
     */
    void sendMessage(const QString &message);

    /**
     * @brief Sélectionne un rôle pour le joueur.
     * @param role Le rôle à sélectionner.
     */
    void selectRole(const QString &role);

    /**
     * @brief Envoie la position de la raquette en données binaires.
     * @param paddleY La position Y de la raquette.
     */
    void sendPaddlePositionBinary(float paddleY);

    /**
     * @brief Simule les données de la raquette.
     */
    void simulatePaddleData();

    /**
     * @brief Vérifie la position de la raquette et envoie les données binaires.
     */
    void checkPaddlePosition();

    /**
     * @brief Démarre le timer pour vérifier la position de la raquette.
     */
    void startCheckPaddleTimer();

    /**
     * @brief Timer pour vérifier la position de la raquette.
     */
    QTimer *checkPaddleTimer;

    /**
     * @brief Démarre le timer pour vérifier la position de la balle.
     */
    void startCheckBallTimer();

    /**
     * @brief Vérifie la position de la balle et envoie les données binaires.
     */
    void checkBallPosition();

    /**
     * @brief Timer pour vérifier la position de la balle.
     */
    QTimer *checkBallTimer;

    /**
     * @brief Envoie la position de la balle en données binaires.
     * @param ballY La position Y de la balle.
     * @param ballX La position X de la balle.
     */
    void sendBallPositionBinary(float ballY, float ballX);

    /**
     * @brief Identifiant du joueur.
     */
    int playerId;

signals:
    /**
     * @brief Signal émis lorsque des slots disponibles sont reçus.
     * @param slots La liste des slots disponibles.
     */
    void availableSlotsReceived(const QStringList &slots);

    /**
     * @brief Signal émis lors de la sélection d'un rôle.
     * @param role Le rôle sélectionné.
     * @param playerId L'identifiant du joueur.
     * @param join Indique si le joueur a rejoint.
     */
    void roleEmit(const QString &role, int playerId, bool join);

    /**
     * @brief Signal émis lorsque la partie commence.
     */
    void gameStartedEmit();

    /**
     * @brief Signal émis lorsque des informations sur le jeu sont reçues.
     * @param gameMode Le mode de jeu.
     */
    void gameInfoReceived(const QString &gameMode);

private slots:
    /**
     * @brief Slot appelé lorsque des données sont reçues du serveur.
     */
    void onDataReceived();

private:
    /**
     * @brief Socket TCP pour la communication avec le serveur.
     */
    QTcpSocket *tcpSocket;

    /**
     * @brief Adresse du serveur.
     */
    QHostAddress serverAddress;

    /**
     * @brief Dernière position Y de la raquette.
     */
    float lastPaddleY = -1;

    /**
     * @brief Timer pour la simulation.
     */
    QTimer *simulationTimer;

    /**
     * @brief Position Y simulée de la raquette.
     */
    float simulationPaddleY { 0.0f };

    /**
     * @brief Dernière position Y de la balle.
     */
    float lastBallY = -1;

    /**
     * @brief Dernière position X de la balle.
     */
    float lastBallX = -1;

    /**
     * @brief Liste des gestionnaires d'événements côté client.
     */
    std::vector<std::unique_ptr<ClientEventHandler>> handlers;
};

#endif // GAMECLIENT_H