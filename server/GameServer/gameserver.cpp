#include "GameServer.h"
#include <QDebug>

GameServer::GameServer(QObject *parent)
    : QObject(parent),
      udpSocket(new QUdpSocket(this)),
      maxPlayers(2),
      currentPlayers(0),
      gameMode(1),
      autoAssignRoles(false)
{
    connect(udpSocket, &QUdpSocket::readyRead, this, &GameServer::onDataReceived);
}

void GameServer::startServer(quint16 port, int mode, bool autoAssign)
{
    qDebug() << "print";
    gameMode = mode;
    maxPlayers = (mode == 1) ? 2 : 4;
    autoAssignRoles = autoAssign;

    rolesList.clear();
    roleTaken.clear();
    if (mode == 1) {
        rolesList << "p1" << "p2";
    } else {
        rolesList << "p1" << "p2" << "p3" << "p4";
    }
    for (const QString &r : rolesList) {
        roleTaken[r] = false;
    }

    if (udpSocket->bind(QHostAddress::Any, port)) {
        qDebug() << "Serveur UDP démarré sur le port" << port << "Mode:" << ((mode == 1) ? "1vs1" : "2vs2");
    } else {
        qDebug() << "Erreur: impossible de démarrer le serveur";
    }
}

void GameServer::onDataReceived()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
        QString message = QString::fromUtf8(buffer);
        qDebug() << "SS | Reçu:" << message << "de" << sender.toString() << ":" << senderPort;


        if (message == "JOIN") {
            if (currentPlayers < maxPlayers) {
                int playerId = currentPlayers + 1;
                PlayerInfo info;
                info.ip = sender;
                info.port = senderPort;
                info.role = "";
                info.ready = false;
                players[playerId] = info;
                currentPlayers++;
                qDebug() << "Nouveau joueur assigné:" << playerId;

                sendMessageToPlayer(playerId, "ASSIGN_ID " + QString::number(playerId));

                sendWaitingRoomInfo(playerId);
                sendMessageToAll("PLAYER_JOINED " + QString::number(playerId));

                if (autoAssignRoles) {
                    for (const QString &r : rolesList) {
                        if (!roleTaken[r]) {
                            players[playerId].role = r;
                            players[playerId].ready = true;
                            roleTaken[r] = true;
                            sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + r);
                            sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + r);
                            break;
                        }
                    }
                    checkAndStartGame();
                }
            } else {
                udpSocket->writeDatagram("FULL", sender, senderPort);
            }
        }
        else if (message.startsWith("SELECT_ROLE ")) {
            QString chosenRole = message.section(' ', 1, 1);
            int playerId = findPlayerId(sender, senderPort);
            if (playerId != -1) {
                if (!roleTaken.value(chosenRole, true)) {
                    players[playerId].role = chosenRole;
                    players[playerId].ready = true;
                    roleTaken[chosenRole] = true;
                    sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + chosenRole);
                    sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + chosenRole);
                    updateWaitingRoomForAll();
                    checkAndStartGame();
                } else {
                    sendMessageToPlayer(playerId, "ERROR Role not available");
                }
            }
        }
    }
}

void GameServer::sendMessageToAll(const QString &message)
{   
    qDebug() << "  ";
    qDebug() << "  ";
    qDebug() << "Envoi à tous:" << message;
    QByteArray data = message.toUtf8();
    for (auto it = players.begin(); it != players.end(); ++it) {
        qDebug() << "Envoi à" << it.key() << "->" << data;
        udpSocket->writeDatagram(data, it.value().ip, it.value().port);
    }
    qDebug() << "Envoi terminé";
    qDebug() << "  ";
    qDebug() << "  ";
}

void GameServer::sendMessageToPlayer(int playerId, const QString &message)
{
    qDebug() << "  ";
    qDebug() << "Envoi au joueur" << playerId << "->" << message;
    qDebug() << "  ";
    if (players.contains(playerId)) {
        QByteArray data = message.toUtf8();
        udpSocket->writeDatagram(data, players[playerId].ip, players[playerId].port);
    }
}

void GameServer::sendWaitingRoomInfo(int playerId)
{
    QStringList available;
    for (const QString &r : rolesList) {
        if (!roleTaken[r])
            available << r;
    }
    QString msgSlots = "AVAILABLE_SLOTS " + available.join(" ");
    sendMessageToPlayer(playerId, msgSlots);
    sendMessageToPlayer(playerId, "FREE_COUNT " + QString::number(available.size()));
}

void GameServer::updateWaitingRoomForAll()
{
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (!it.value().ready) {
            sendWaitingRoomInfo(it.key());
        }
    }
}

void GameServer::checkAndStartGame()
{
    if (players.size() == maxPlayers) {
        bool allReady = true;
        for (const PlayerInfo &info : players) {
            if (!info.ready) {
                allReady = false;
                break;
            }
        }
        if (allReady) {
            sendMessageToAll("GAME_START");
            QString gameInfo = "GAME_INFO";
            for (auto it = players.begin(); it != players.end(); ++it) {
                gameInfo += " " + QString::number(it.key()) + ":" + it.value().role;
            }
            sendMessageToAll(gameInfo);
        }
    }
}

int GameServer::findPlayerId(const QHostAddress &ip, quint16 port)
{
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it.value().ip == ip && it.value().port == port)
            return it.key();
    }
    return -1;
}
