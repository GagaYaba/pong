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

    // Envoi de messages au serveur
    void sendMessage(const QString &message);

    // Sélectionner un rôle
    void selectRole(const QString &role);
    // Envoyer la position de la raquette
    void sendPaddlePosition(float paddleY);
    int playerId;

signals:
    void availableSlotsReceived(const QStringList &slots);
    void roleEmit(const QString &role);
    void gameStartedEmit();  // Signal pour informer que le jeu a commencé


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
