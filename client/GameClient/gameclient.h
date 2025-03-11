#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <memory>
#include <vector>

#include "ClientEventHandler.h"
#include "ClientEventHandlerFactory.h"

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
    void sendPaddlePosition(float paddleY);
    int playerId;

signals:
    void availableSlotsReceived(const QStringList &slots);
    void gameStart();
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

    // Liste des gestionnaires d'événements
    std::vector<std::unique_ptr<ClientEventHandler>> handlers;
};

#endif // GAMECLIENT_H
