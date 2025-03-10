#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <memory>
#include <vector>

#include "ClientEventHandler.h"
#include "ClientEventHandlerFactory.h"

#include <QTimer>
#include <QDataStream>


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
    void sendPaddlePositionBinary(float paddleY);
    void startPaddleSimulation(); // si vous voulez démarrer explicitement la simulation

    int playerId;

signals:
    void availableSlotsReceived(const QStringList &slots);
    void roleEmit(const QString &role);

private slots:
    // Gestion des données reçues du serveur
    void onDataReceived();
    void simulatePaddleData();


private:
    QTcpSocket *tcpSocket;
    QHostAddress serverAddress;

    float lastPaddleY { -1.0f }; // valeur initiale
    QTimer *simulationTimer;
    float simulationPaddleY { 0.0f };

    // Liste des gestionnaires d'événements
    std::vector<std::unique_ptr<ClientEventHandler>> handlers;
};

#endif // GAMECLIENT_H
