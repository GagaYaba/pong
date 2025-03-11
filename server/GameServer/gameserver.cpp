#include "gameserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <memory>
#include <vector>

// ==============================
// Interface du BinaryEventHandlerS
// ==============================
class BinaryEventHandlerS
{
public:
    virtual ~BinaryEventHandlerS() {}
    // Retourne vrai si ce handler peut traiter le message contenu dans le flux
    virtual bool canHandle(QDataStream &stream) const = 0;
    // Traite l'événement en passant le GameServer, le socket client et le flux de données
    virtual void handle(GameServer *server, QTcpSocket *clientSocket, QDataStream &stream) = 0;
};

// ============================================
// Handler pour le message binaire "Paddle Move"
// ============================================
class PaddleMoveBinaryEventHandler : public BinaryEventHandlerS
{
public:
    bool canHandle(QDataStream &stream) const override
    {
        // Sauvegarde de la position courante du flux pour ne pas consommer les données
        qint64 pos = stream.device()->pos();
        quint8 messageType;
        stream >> messageType;
        stream.device()->seek(pos);
        return messageType == 1; // 1 correspond au type "Paddle Move"
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, QDataStream &stream) override
    {
        quint8 messageType;
        stream >> messageType;
        qint32 playerId;
        float paddleY;
        stream >> playerId;
        stream >> paddleY;

        qDebug() << "Serveur | Reçu Paddle Move binaire - PlayerID:" << playerId << "Position Y:" << paddleY;

        server->sendPaddlePosition(playerId, paddleY);
    }
};

class BallMoveBinaryEventHandler : public BinaryEventHandlerS
{   
public:
    bool canHandle(QDataStream &stream) const override
    {
        // Sauvegarde de la position courante du flux pour ne pas consommer les données
        qint64 pos = stream.device()->pos();
        quint8 messageType;
        stream >> messageType;
        stream.device()->seek(pos);
        return messageType == 2; // 2 correspond au type "Ball Move"
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, QDataStream &stream) override
    {
        quint8 messageType;
        stream >> messageType;
        qint32 playerId;
        float ballY;
        float ballX;
        stream >> playerId;
        stream >> ballY;
        stream >> ballX;

        qDebug() << "Serveur | Reçu Ball Move binaire - PlayerID:" << playerId << "Position Y:" << ballY << "Position X:" << ballX;

        server->sendBallPosition(playerId, ballY, ballX);
    }
};

// ============================================
// Factory pour créer les BinaryEventHandlerS
// ============================================
class BinaryEventHandlerFactoryS
{
public:
    static std::vector<std::unique_ptr<BinaryEventHandlerS>> createHandlers()
    {
        std::vector<std::unique_ptr<BinaryEventHandlerS>> handlers;
        handlers.push_back(std::make_unique<PaddleMoveBinaryEventHandler>());
        handlers.push_back(std::make_unique<BallMoveBinaryEventHandler>());
        // Vous pouvez ajouter d'autres handlers binaires ici
        return handlers;
    }
};

// ==============================
// Interface de l'EventHandler
// ==============================
class EventHandler
{
public:
    virtual ~EventHandler() {}
    // Retourne vrai si ce handler peut traiter le message
    virtual bool canHandle(const QString &message) const = 0;
    // Traite l'événement en passant le GameServer, le socket client et le message
    virtual void handle(GameServer *server, QTcpSocket *clientSocket, const QString &message) = 0;
};

// =====================================
// Handler pour l'événement "JOIN"
// =====================================
class JoinEventHandler : public EventHandler
{
public:
    bool canHandle(const QString &message) const override
    {
        return message.trimmed() == "JOIN";
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, const QString & /*message*/) override
    {
        // Code repris de la branche "JOIN" d'origine
        if (server->currentPlayers < server->maxPlayers)
        {
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

            if (server->autoAssignRoles)
            {
                for (const QString &r : server->rolesList)
                {
                    if (!server->roleTaken[r])
                    {
                        server->players[playerId].role = r;
                        server->roleTaken[r] = true;
                        server->sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + r);
                        server->sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + r);
                        break;
                    }
                }
                server->checkAndStartGame();
            }
        }
        else
        {
            clientSocket->write("FULL");
            clientSocket->disconnectFromHost();
        }
    }
};

// ============================================
// Handler pour l'événement "SELECT_ROLE"
// ============================================
class SelectRoleEventHandler : public EventHandler
{
public:
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("SELECT_ROLE ");
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, const QString &message) override
    {
        QString chosenRole = message.section(' ', 1, 1); // ex: "p1"
        int playerId = server->findPlayerId(clientSocket->peerAddress(), clientSocket->peerPort());
        if (playerId != -1)
        {
            if (!server->roleTaken.value(chosenRole, true))
            {
                server->players[playerId].role = chosenRole;
                server->roleTaken[chosenRole] = true;
                server->sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + chosenRole);
                server->sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + chosenRole);
                // server->updateWaitingRoomForAll();
                server->checkAndStartGame();
            }
            else
            {
                server->sendMessageToPlayer(playerId, "ERROR Role not available");
            }
        }
    }
};

// ============================================
// Handler pour l'événement "START_GAME"
// ============================================
class StartGameEventHandler : public EventHandler
{
public:
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("START_GAME");
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, const QString &message) override
    {
        QString gameModeString = (server->gameMode == 1) ? "OneVOne" : "TwoVTwo";
        QString messageGameInfo = "GAME_INFO " + gameModeString;

        server->sendMessageToAll(messageGameInfo);
    }
};

// ============================================
// Handler pour l'événement "READY"
// ============================================
class ReadyEventHandler : public EventHandler
{
public:
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("READY ");
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, const QString &message) override
    {
        QString playerIdSec = message.section(' ', 1, 1); // ex: "p1"
        int playerId = server->findPlayerId(clientSocket->peerAddress(), clientSocket->peerPort());
        if (playerId != -1 && playerId == playerIdSec.toInt())
        {
            server->players[playerId].ready = true;
            server->checkAndStartGame();
        }
    }
};

// ============================================
// Factory pour créer les EventHandlers
// ============================================
class EventHandlerFactory
{
public:
    static std::vector<std::unique_ptr<EventHandler>> createHandlers()
    {
        std::vector<std::unique_ptr<EventHandler>> handlers;
        handlers.push_back(std::make_unique<JoinEventHandler>());
        handlers.push_back(std::make_unique<SelectRoleEventHandler>());
        handlers.push_back(std::make_unique<StartGameEventHandler>());
        handlers.push_back(std::make_unique<ReadyEventHandler>());
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
    if (mode == 1)
    {
        rolesList << "p1" << "p2";
    }
    else
    {
        rolesList << "p1" << "p2" << "p3" << "p4";
    }
    for (const QString &r : rolesList)
    {
        roleTaken[r] = false;
    }

    QList<quint16> ports = {27460, 25518, 27718, 28147, 27808, 26897, 29102, 25499, 27520, 27392};
    bool serverStarted = false;

    for (quint16 port : ports)
    {
        if (tcpServer->listen(QHostAddress::Any, port))
        {
            qDebug() << "Serveur TCP démarré sur le port" << port << "Mode:" << ((mode == 1) ? "OneVOne" : "TwoVTwo");
            serverStarted = true;
            break;
        }
        else
        {
            qDebug() << "Le port" << port << "n'est pas disponible, test suivant...";
        }
    }

    if (!serverStarted)
    {
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
    if (clientSocket)
    {
        // On regarde le contenu sans le retirer de la file d'attente
        QByteArray peekBuffer = clientSocket->peek(clientSocket->bytesAvailable());

        if (peekBuffer.endsWith('\n'))
        {
            // Traitement du message en chaîne de caractères
            QByteArray buffer = clientSocket->readAll();
            QString message = QString::fromUtf8(buffer).trimmed();
            qDebug() << "Serveur | Reçu (string):" << message
                     << "de" << clientSocket->peerAddress().toString()
                     << ":" << clientSocket->peerPort();

            // Récupère la liste des handlers string depuis la factory existante
            auto handlers = EventHandlerFactory::createHandlers();
            for (const auto &handler : handlers)
            {
                if (handler->canHandle(message))
                {
                    handler->handle(this, clientSocket, message);
                    break; // On arrête dès qu'un handler a traité l'évènement
                }
            }
        }
        else
        {
            // Traitement du message binaire
            QByteArray buffer = clientSocket->readAll();
            QDataStream stream(buffer);
            stream.setByteOrder(QDataStream::BigEndian);
            stream.setVersion(QDataStream::Qt_6_0);

            // Récupère la liste des handlers binaires depuis la factory dédiée
            auto binaryHandlers = BinaryEventHandlerFactoryS::createHandlers();
            bool handled = false;
            for (const auto &handler : binaryHandlers)
            {
                if (handler->canHandle(stream))
                {
                    handler->handle(this, clientSocket, stream);
                    handled = true;
                    break;
                }
            }
            if (!handled)
            {
                qDebug() << "Serveur | Message binaire non reconnu";
            }
        }
    }
}

void GameServer::onDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket)
    {
        int playerId = findPlayerId(clientSocket->peerAddress(), clientSocket->peerPort());
        if (playerId != -1)
        {
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
    for (auto it = players.begin(); it != players.end(); ++it)
    {
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
    if (players.contains(playerId))
    {
        QByteArray data = message.toUtf8();
        players[playerId].socket->write(data + "\n");
    }
}

void GameServer::sendBinaryToPlayer(int playerId, quint8 messageType, const QByteArray &payload) {
    if (!players.contains(playerId)) {
        qDebug() << "Serveur | Erreur: Joueur" << playerId << "non trouvé.";
        return;
    }

    QTcpSocket *clientSocket = players[playerId].socket;
    if (!clientSocket || clientSocket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "Serveur | Erreur: Socket du joueur" << playerId << "non connectée.";
        return;
    }

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setVersion(QDataStream::Qt_6_0);

    stream << messageType;  // Type du message
    data.append(payload);   // Ajouter la charge utile (payload)

    clientSocket->write(data);
    clientSocket->flush();

    qDebug() << "Serveur | Message binaire envoyé (Type:" << messageType << ") au joueur" << playerId;
}

void GameServer::sendBinaryToAllExcept(int excludedPlayerId, quint8 messageType, const QByteArray &payload) {
    for (auto playerId : players.keys()) {
        if (playerId != excludedPlayerId) {
            sendBinaryToPlayer(playerId, messageType, payload);
        }
    }
}

void GameServer::broadcastBinaryData(quint8 messageType, const QByteArray &payload) {
    for (auto playerId : players.keys()) {
        sendBinaryToPlayer(playerId, messageType, payload);
    }
}

void GameServer::sendPaddlePosition(int playerId, float paddleY) {
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setVersion(QDataStream::Qt_6_0);

    stream << static_cast<qint32>(playerId); // ID du joueur
    stream << paddleY; // Position Y du paddle

    sendBinaryToAllExcept(playerId, playerId, payload);
}

void GameServer::sendBallPosition(int playerId, float ballY, float ballX) {
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setVersion(QDataStream::Qt_6_0);

    stream << static_cast<qint32>(playerId); // ID du joueur
    stream << ballY; // Position Y de la balle
    stream << ballX; // Position X de la balle

    sendBinaryToAllExcept(playerId, 5, payload);
}

void GameServer::sendWaitingRoomInfo(int playerId)
{
    QStringList available;
    for (const QString &r : rolesList)
    {
        if (!roleTaken[r])
            available << r;
    }
    QString msgSlots = "AVAILABLE_SLOTS " + available.join(" ");
    sendMessageToPlayer(playerId, msgSlots);
}

void GameServer::updateWaitingRoomForAll()
{
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        if (!it.value().ready)
        {
            sendWaitingRoomInfo(it.key());
        }
    }
}

void GameServer::checkAndStartGame()
{
    if (players.size() == maxPlayers)
    {
        bool allReady = true;
        for (const PlayerInfo &info : players)
        {
            if (!info.ready)
            {
                allReady = false;
                break;
            }
        }
        if (allReady)
        {
            sendMessageToAll("GAME_START");
        }
    }
}

int GameServer::findPlayerId(const QHostAddress &ip, quint16 port)
{
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        if (it.value().socket->peerAddress() == ip && it.value().socket->peerPort() == port)
            return it.key();
    }
    return -1;
}
