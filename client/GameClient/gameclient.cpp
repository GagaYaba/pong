#include "GameClient.h"
#include <QDebug>

GameClient::GameClient(QObject *parent)
    : QObject(parent),
      udpSocket(new QUdpSocket(this)),
      playerId(-1)
{
    connect(udpSocket, &QUdpSocket::readyRead, this, &GameClient::onDataReceived);
}

void GameClient::connectToServer(const QHostAddress &serverAddr, quint16 port)
{
    this->serverAddress = serverAddr;
    this->serverPort = port;

    sendMessage("JOIN", serverAddr, port);
    qDebug() << "Demande de connexion envoyée au serveur...";
}

void GameClient::sendMessage(const QString &message, const QHostAddress &serverAddr, quint16 port)
{
    QByteArray data = message.toUtf8();
    udpSocket->writeDatagram(data, serverAddr, port);
}

void GameClient::selectRole(const QString &role)
{
    QString message = "SELECT_ROLE " + role;
    sendMessage(message, serverAddress, serverPort);
    qDebug() << "Demande de sélection de rôle envoyée:" << role;
}

void GameClient::sendPaddlePosition(float paddleY)
{
    if (paddleY != lastPaddleY) {
        lastPaddleY = paddleY;
        QString message = "PADDLE " + QString::number(playerId) + " " + QString::number(paddleY);
        sendMessage(message, serverAddress, serverPort);
    }
}

void GameClient::onDataReceived()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        qint64 dataSize = udpSocket->pendingDatagramSize();
        if (dataSize <= 0)
            continue;

        buffer.resize(dataSize);
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        QString message = QString::fromUtf8(buffer);
        QStringList parts = message.split(" ");

        qDebug() << "CC (" << playerId << ") | Message reçu du serveur:" << message;

        if (parts[0] == "ROLE_ASSIGNED") {
            if (parts.size() > 1) {
                QString role = parts[1];
                qDebug() << "Rôle attribué:" << role;
            } else {
                qDebug() << "Erreur: message ROLE_ASSIGNED mal formé";
            }
        }
        else if (parts[0] == "ASSIGN_ID") {
            // Exemple : "ASSIGN_ID 1"
            if (parts.size() > 1) {
                playerId = parts[1].toInt();
                qDebug() << "ID attribué:" << playerId;
            } else {
                qDebug() << "Erreur: message ASSIGN_ID mal formé";
            }
        }
        else if (parts[0] == "AVAILABLE_SLOTS") {
            parts.removeFirst();
            qDebug() << "Slots disponibles:" << parts;
        }
        else if (parts[0] == "FREE_COUNT") {
            if (parts.size() > 1) {
                int freeCount = parts[1].toInt();
                qDebug() << "Nombre de slots libres:" << freeCount;
            } else {
                qDebug() << "Erreur: message FREE_COUNT mal formé";
            }
        }
        else if (parts[0] == "PLAYER_JOINED") {
            if (parts.size() > 1) {
                int joinedPlayerId = parts[1].toInt();
                qDebug() << "Nouveau joueur rejoint:" << joinedPlayerId;
            }
        }
        else if (parts[0] == "PLAYER_UPDATED") {
            if (parts.size() > 2) {
                int updatedPlayerId = parts[1].toInt();
                QString role = parts[2];
                qDebug() << "Joueur" << updatedPlayerId << "mis à jour avec le rôle:" << role;
            }
        }
        else if (parts[0] == "GAME_START") {
            qDebug() << "La partie commence!";
        }
        else if (parts[0] == "GAME_INFO") {
            parts.removeFirst();
            qDebug() << "Informations de la partie:";
            for (const QString &info : parts) {
                qDebug() << info;
            }
        }
        else if (parts[0] == "FULL") {
            qDebug() << "Serveur complet, impossible de se connecter";
        }
        else {
            qDebug() << "Message inconnu reçu:" << message;
        }
    }
}
