#include "GameServer.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>

GameServer::GameServer(QObject *parent)
    : QObject(parent),
      tcpServer(new QTcpServer(this)),
      maxPlayers(2),
      currentPlayers(0),
      gameMode(1),
      autoAssignRoles(false)
{
    connect(tcpServer, &QTcpServer::newConnection, this, &GameServer::onNewConnection);
}

void GameServer::startServer(int mode, bool autoAssign)
{
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

    QList<quint16> ports = {27460, 25518, 27718, 28147, 27808, 26897, 29102, 25499, 27520, 27392};
    bool serverStarted = false;

    for (quint16 port : ports) {
        if (tcpServer->listen(QHostAddress::Any, port)) {
            qDebug() << "Serveur TCP démarré sur le port" << port << "Mode:" << ((mode == 1) ? "1vs1" : "2vs2");
            serverStarted = true;
            break;
        } else {
            qDebug() << "Le port" << port << "n'est pas disponible, test suivant...";
        }
    }

    if (!serverStarted) {
        qDebug() << "Erreur: aucun des ports disponibles n'a pu être utilisé.";
    }
}

void GameServer::onNewConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onDisconnected);

    if (currentPlayers < maxPlayers) {
        int playerId = currentPlayers + 1;
        PlayerInfo info;
        info.socket = clientSocket;
        info.role = "";
        info.ready = false;
        players[playerId] = info;
        currentPlayers++;
        qDebug() << "Nouveau joueur assigné:" << playerId;

        sendMessageToPlayer(playerId, "ASSIGN_ID " + QString::number(playerId));

        // Envoyer les infos de la salle d'attente au nouveau joueur
        sendWaitingRoomInfo(playerId);
        // Notifier tous les joueurs
        sendMessageToAll("PLAYER_JOINED " + QString::number(playerId));

        // Si attribution automatique des rôles activée, on attribue directement le premier rôle disponible
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
        // Partie pleine
        clientSocket->write("FULL");
        clientSocket->disconnectFromHost();
    }
}

void GameServer::onDataReceived()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        QByteArray buffer = clientSocket->readAll();
        QString message = QString::fromUtf8(buffer);
        QHostAddress sender = clientSocket->peerAddress();
        quint16 senderPort = clientSocket->peerPort();
        qDebug() << "SS | Reçu:" << message << "de" << sender.toString() << ":" << senderPort;

        if (message == "JOIN") {
            if (currentPlayers < maxPlayers) {
                int playerId = currentPlayers + 1;
                PlayerInfo info;
                info.socket = clientSocket;
                info.role = "";
                info.ready = false;
                players[playerId] = info;
                currentPlayers++;
                qDebug() << "Nouveau joueur assigné:" << playerId;

                sendMessageToPlayer(playerId, "ASSIGN_ID " + QString::number(playerId));

                // Envoyer les infos de la salle d'attente au nouveau joueur
                sendWaitingRoomInfo(playerId);
                // Notifier tous les joueurs
                sendMessageToAll("PLAYER_JOINED " + QString::number(playerId));

                // Si attribution automatique des rôles activée, on attribue directement le premier rôle disponible
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
                // Partie pleine
                clientSocket->write("FULL");
            }
        }
        // Cas de la sélection d'un rôle par un joueur (attribution manuelle)
        else if (message.startsWith("SELECT_ROLE ")) {
            QString chosenRole = message.section(' ', 1, 1); // ex: "p1"
            int playerId = findPlayerId(sender, senderPort);
            if (playerId != -1) {
                if (!roleTaken.value(chosenRole, true)) {
                    players[playerId].role = chosenRole;
                    players[playerId].ready = true;
                    roleTaken[chosenRole] = true;
                    sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + chosenRole);
                    sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + chosenRole);
                    // Mettre à jour la salle d'attente pour tous (pour actualiser la liste des slots disponibles)
                    updateWaitingRoomForAll();
                    checkAndStartGame();
                } else {
                    sendMessageToPlayer(playerId, "ERROR Role not available");
                }
            }
        }
        // Vous pouvez ajouter ici d'autres types de messages (ex: READY, MESSAGE, etc.)
    }
}

void GameServer::onDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        // Trouver l'ID du joueur en fonction de l'adresse et du port
        int playerId = findPlayerId(clientSocket->peerAddress(), clientSocket->peerPort());
        if (playerId != -1) {
            players.remove(playerId);
            currentPlayers--;
            qDebug() << "Joueur déconnecté:" << playerId;
            sendMessageToAll("PLAYER_LEFT " + QString::number(playerId));
        }
        clientSocket->deleteLater();
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
        it.value().socket->write(data + "\n");
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
        players[playerId].socket->write(data + "\n");
    }
}

// Envoie la liste des rôles disponibles et le nombre de slots libres au joueur concerné
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

// Met à jour la salle d'attente pour tous les joueurs qui n'ont pas encore choisi de rôle
void GameServer::updateWaitingRoomForAll()
{
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (!it.value().ready) {
            sendWaitingRoomInfo(it.key());
        }
    }
}

// Vérifie si tous les joueurs ont choisi leur rôle et démarre la partie le cas échéant
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
            // Envoi d'une info de configuration (ex: la liste des joueurs et leurs rôles)
            QString gameInfo = "GAME_INFO";
            for (auto it = players.begin(); it != players.end(); ++it) {
                gameInfo += " " + QString::number(it.key()) + ":" + it.value().role;
            }
            sendMessageToAll(gameInfo);
        }
    }
}

// Permet de retrouver l'ID du joueur grâce à son adresse IP et son port
int GameServer::findPlayerId(const QHostAddress &ip, quint16 port)
{
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it.value().socket->peerAddress() == ip && it.value().socket->peerPort() == port)
            return it.key();
    }
    return -1;
}
