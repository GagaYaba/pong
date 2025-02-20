#include "gameserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <memory>
#include <vector>

// ==============================
// Interface de l'EventHandler
// ==============================
class EventHandler {
public:
    virtual ~EventHandler() {}
    // Retourne vrai si ce handler peut traiter le message
    virtual bool canHandle(const QString &message) const = 0;
    // Traite l'événement en passant le GameServer, le socket client et le message
    virtual void handle(GameServer* server, QTcpSocket* clientSocket, const QString &message) = 0;
};

// =====================================
// Handler pour l'événement "JOIN"
// =====================================
class JoinEventHandler : public EventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.trimmed() == "JOIN";
    }

    void handle(GameServer* server, QTcpSocket* clientSocket, const QString & /*message*/) override {
        // Code repris de la branche "JOIN" d'origine
        if (server->currentPlayers < server->maxPlayers) {
            int playerId = server->currentPlayers + 1;
            PlayerInfo info;
            info.socket = clientSocket;
            info.role = "";
            info.ready = false;
            server->players[playerId] = info;
            server->currentPlayers++;
            qDebug() << "Nouveau joueur assigné:" << playerId;

            server->sendMessageToPlayer(playerId, "ASSIGN_ID " + QString::number(playerId));
            server->sendWaitingRoomInfo(playerId);
            server->sendMessageToAll("PLAYER_JOINED " + QString::number(playerId));

            if (server->autoAssignRoles) {
                for (const QString &r : server->rolesList) {
                    if (!server->roleTaken[r]) {
                        server->players[playerId].role = r;
                        server->players[playerId].ready = true;
                        server->roleTaken[r] = true;
                        server->sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + r);
                        server->sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + r);
                        break;
                    }
                }
                server->checkAndStartGame();
            }
        } else {
            clientSocket->write("FULL");
            clientSocket->disconnectFromHost();
        }
    }
};

// ============================================
// Handler pour l'événement "SELECT_ROLE"
// ============================================
class SelectRoleEventHandler : public EventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("SELECT_ROLE ");
    }

    void handle(GameServer* server, QTcpSocket* clientSocket, const QString &message) override {
        QString chosenRole = message.section(' ', 1, 1); // ex: "p1"
        int playerId = server->findPlayerId(clientSocket->peerAddress(), clientSocket->peerPort());
        if (playerId != -1) {
            if (!server->roleTaken.value(chosenRole, true)) {
                server->players[playerId].role = chosenRole;
                server->players[playerId].ready = true;
                server->roleTaken[chosenRole] = true;
                server->sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + chosenRole);
                server->sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + chosenRole);
                server->updateWaitingRoomForAll();
                server->checkAndStartGame();
            } else {
                server->sendMessageToPlayer(playerId, "ERROR Role not available");
            }
        }
    }
};

// ============================================
// Factory pour créer les EventHandlers
// ============================================
class EventHandlerFactory {
public:
    static std::vector<std::unique_ptr<EventHandler>> createHandlers() {
        std::vector<std::unique_ptr<EventHandler>> handlers;
        handlers.push_back(std::make_unique<JoinEventHandler>());
        handlers.push_back(std::make_unique<SelectRoleEventHandler>());
        // On peut ajouter ici d'autres handlers pour de nouveaux types d'évènements
        return handlers;
    }
};

// ==============================
// Implémentation de GameServer
// ==============================
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
}

void GameServer::onDataReceived()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        QByteArray buffer = clientSocket->readAll();
        QString message = QString::fromUtf8(buffer).trimmed();
        qDebug() << "SS | Reçu:" << message
                 << "de" << clientSocket->peerAddress().toString()
                 << ":" << clientSocket->peerPort();

        // Récupère la liste des handlers depuis la factory
        auto handlers = EventHandlerFactory::createHandlers();
        // Parcours de chaque handler dans une boucle for
        for (const auto &handler : handlers) {
            if (handler->canHandle(message)) {
                handler->handle(this, clientSocket, message);
                break; // On arrête dès qu'un handler a traité l'évènement
            }
        }
    }
}

void GameServer::onDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
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

void GameServer::sendWaitingRoomInfo(int playerId)
{
    QStringList available;
    for (const QString &r : rolesList) {
        if (!roleTaken[r])
            available << r;
    }
    QString msgSlots = "AVAILABLE_SLOTS " + available.join(" ");
    sendMessageToPlayer(playerId, msgSlots);
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
        if (it.value().socket->peerAddress() == ip && it.value().socket->peerPort() == port)
            return it.key();
    }
    return -1;
}
