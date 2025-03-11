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

class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);

    // Connexion au serveur
    void connectToServer(const QHostAddress &serverAddr);
    void startGame();
    // Envoi de messages au serveur
    void sendMessage(const QString &message);

    // Sélectionner un rôle
    void selectRole(const QString &role);
    // Envoyer la position de la raquette
    void sendPaddlePositionBinary(float paddleY);
    void simulatePaddleData();
    void checkPaddlePosition();
    void startCheckPaddleTimer();
    QTimer *checkPaddleTimer;
    void startCheckBallTimer();
    void checkBallPosition();
    QTimer *checkBallTimer;
    void sendBallPositionBinary(float ballY, float ballX);



    int playerId;

signals:
    void availableSlotsReceived(const QStringList &slots);
    void roleEmit(const QString &role, int playerId, bool join);
    void gameStartedEmit();
    void gameInfoReceived(const QString &gameMode);


private slots:
    // Gestion des données reçues du serveur
    void onDataReceived();


private:
    QTcpSocket *tcpSocket;
    QHostAddress serverAddress;
    float lastPaddleY = -1;
    QTimer *simulationTimer;
    float simulationPaddleY { 0.0f };
    float lastBallY = -1;
    float lastBallX = -1;

    // Liste des gestionnaires d'événements
    
    std::vector<std::unique_ptr<ClientEventHandler>> handlers;
};

#endif // GAMECLIENT_H
