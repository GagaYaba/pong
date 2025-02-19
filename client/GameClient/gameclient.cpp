#include "GameClient.h"
#include <QDebug>
#include <QString>

GameClient::GameClient(QObject *parent)
    : QObject(parent),
      tcpSocket(new QTcpSocket(this)),   // Initialisation du socket TCP
      playerId(-1)
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &GameClient::onDataReceived);
}

void GameClient::connectToServer(const QHostAddress &serverAddr, quint16 port)
{
    // Sauvegarder l'adresse et le port du serveur pour les messages ultérieurs
    this->serverAddress = serverAddr;
    this->serverPort = port;

    // Connexion au serveur TCP
    tcpSocket->connectToHost(serverAddr, port);
    if (tcpSocket->waitForConnected(3000)) {
        qDebug() << "Connexion établie avec le serveur...";
        // sendMessage("JOIN");  // Envoi du message de demande de connexion
    } else {
        qDebug() << "Impossible de se connecter au serveur";
    }
}

void GameClient::sendMessage(const QString &message)
{
    QByteArray data = message.toUtf8();
    if (tcpSocket->state() == QTcpSocket::ConnectedState) {
        tcpSocket->write(data);
        tcpSocket->flush();   // Assurez-vous que les données sont envoyées immédiatement
        qDebug() << "Message envoyé:" << message;
    } else {
        qDebug() << "Erreur: Connexion non établie";
    }
}

void GameClient::selectRole(const QString &role)
{
    // Envoi d'une demande de sélection de rôle
    QString message = "SELECT_ROLE " + role;
    sendMessage(message);
    qDebug() << "Demande de sélection de rôle envoyée:" << role;
}

void GameClient::sendPaddlePosition(float paddleY)
{
    if (paddleY != lastPaddleY) { // N'envoie que si la position a changé
        lastPaddleY = paddleY;
        QString message = "PADDLE " + QString::number(playerId) + " " + QString::number(paddleY);
        sendMessage(message);
    }
}

void GameClient::onDataReceived()
{
    QByteArray buffer = tcpSocket->readAll();   // Lire tout ce qui a été reçu
    QString data = QString::fromUtf8(buffer);

    qDebug() << "CC (" << playerId << ") | Message reçu du serveur:" << data;

    QStringList messages = data.split("\n", Qt::SkipEmptyParts);   // Divise les messages sur '\n'

    for (const QString &message : messages) {
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
