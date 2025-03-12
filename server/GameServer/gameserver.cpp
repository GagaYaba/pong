#include "gameserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <memory>
#include <vector>

// ==============================
// Interface du BinaryEventHandlerS
// ==============================
/**
 * @brief Interface pour les gestionnaires d'événements binaires.
 */
class BinaryEventHandlerS
{
public:
    virtual ~BinaryEventHandlerS() {}
    /**
     * @brief Vérifie si le gestionnaire peut traiter le flux de données.
     * @param stream Le flux de données à vérifier.
     * @return true si le gestionnaire peut traiter le flux, false sinon.
     */
    virtual bool canHandle(QDataStream &stream) const = 0;

    /**
     * @brief Traite le flux de données.
     * @param server Le serveur de jeu.
     * @param clientSocket Le socket du client.
     * @param stream Le flux de données à traiter.
     */
    virtual void handle(GameServer *server, QTcpSocket *clientSocket, QDataStream &stream) = 0;
};

// ============================================
// Handler pour le message binaire "Paddle Move"
// ============================================
/**
 * @brief Gestionnaire pour le message binaire "Paddle Move".
 */
class PaddleMoveBinaryEventHandler : public BinaryEventHandlerS
{
public:
    bool canHandle(QDataStream &stream) const override
    {
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

        server->sendPaddlePosition(playerId, paddleY);
    }
};

// ============================================
// Handler pour le message binaire "Ball Move"
// ============================================
/**
 * @brief Gestionnaire pour le message binaire "Ball Move".
 */
class BallMoveBinaryEventHandler : public BinaryEventHandlerS
{
public:
    bool canHandle(QDataStream &stream) const override
    {
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

        server->sendBallPosition(playerId, ballY, ballX);
    }
};

// ============================================
// Factory pour créer les BinaryEventHandlerS
// ============================================
/**
 * @brief Factory pour créer les gestionnaires d'événements binaires.
 */
class BinaryEventHandlerFactoryS
{
public:
    /**
     * @brief Crée une liste de gestionnaires d'événements binaires.
     * @return Un vecteur de gestionnaires d'événements binaires.
     */
    static std::vector<std::unique_ptr<BinaryEventHandlerS>> createHandlers()
    {
        std::vector<std::unique_ptr<BinaryEventHandlerS>> handlers;
        handlers.push_back(std::make_unique<PaddleMoveBinaryEventHandler>());
        handlers.push_back(std::make_unique<BallMoveBinaryEventHandler>());
        return handlers;
    }
};

// ==============================
// Interface de l'EventHandler
// ==============================
/**
 * @brief Interface pour les gestionnaires d'événements.
 */
class EventHandler
{
public:
    virtual ~EventHandler() {}
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message.
     * @param message Le message à vérifier.
     * @return true si le gestionnaire peut traiter le message, false sinon.
     */
    virtual bool canHandle(const QString &message) const = 0;

    /**
     * @brief Traite le message.
     * @param server Le serveur de jeu.
     * @param clientSocket Le socket du client.
     * @param message Le message à traiter.
     */
    virtual void handle(GameServer *server, QTcpSocket *clientSocket, const QString &message) = 0;
};

// =====================================
// Handler pour l'événement "JOIN"
// =====================================
/**
 * @brief Gestionnaire pour l'événement "JOIN".
 */
class JoinEventHandler : public EventHandler
{
public:
    bool canHandle(const QString &message) const override
    {
        return message.trimmed() == "JOIN";
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, const QString & /*message*/) override
    {
        if (server->currentPlayers < server->maxPlayers)
        {
            int playerId = server->currentPlayers + 1;
            PlayerInfo info;
            info.socket = clientSocket;
            info.role = "";
            info.ready = false;
            server->players[playerId] = info;
            server->currentPlayers++;

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
/**
 * @brief Gestionnaire pour l'événement "SELECT_ROLE".
 */
class SelectRoleEventHandler : public EventHandler
{
public:
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("SELECT_ROLE ");
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, const QString &message) override
    {
        QString chosenRole = message.section(' ', 1, 1);
        int playerId = server->findPlayerId(clientSocket->peerAddress(), clientSocket->peerPort());
        if (playerId != -1)
        {
            if (!server->roleTaken.value(chosenRole, true))
            {
                server->players[playerId].role = chosenRole;
                server->roleTaken[chosenRole] = true;
                server->sendMessageToPlayer(playerId, "ROLE_ASSIGNED " + chosenRole);
                server->sendMessageToAll("PLAYER_UPDATED " + QString::number(playerId) + " " + chosenRole);
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
/**
 * @brief Gestionnaire pour l'événement "START_GAME".
 */
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
/**
 * @brief Gestionnaire pour l'événement "READY".
 */
class ReadyEventHandler : public EventHandler
{
public:
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("READY ");
    }

    void handle(GameServer *server, QTcpSocket *clientSocket, const QString &message) override
    {
        QString playerIdSec = message.section(' ', 1, 1);
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
/**
 * @brief Factory pour créer les gestionnaires d'événements.
 */
class EventHandlerFactory
{
public:
    /**
     * @brief Crée une liste de gestionnaires d'événements.
     * @return Un vecteur de gestionnaires d'événements.
     */
    static std::vector<std::unique_ptr<EventHandler>> createHandlers()
    {
        std::vector<std::unique_ptr<EventHandler>> handlers;
        handlers.push_back(std::make_unique<JoinEventHandler>());
        handlers.push_back(std::make_unique<SelectRoleEventHandler>());
        handlers.push_back(std::make_unique<StartGameEventHandler>());
        handlers.push_back(std::make_unique<ReadyEventHandler>());
        return handlers;
    }
};

// ==============================
// Implémentation de GameServer
// ==============================
/**
 * @brief Constructeur de la classe GameServer.
 * @param parent Pointeur vers l'objet parent.
 */
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

/**
 * @brief Démarre le serveur de jeu.
 * @param mode Mode de jeu (1 pour OneVOne, 2 pour TwoVTwo).
 * @param autoAssign Indique si les rôles doivent être assignés automatiquement.
 */
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

/**
 * @brief Gère une nouvelle connexion client.
 */
void GameServer::onNewConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onDisconnected);
}

/**
 * @brief Gère la réception de données d'un client.
 */
void GameServer::onDataReceived()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket)
    {
        QByteArray peekBuffer = clientSocket->peek(clientSocket->bytesAvailable());

        if (peekBuffer.endsWith('\n'))
        {
            QByteArray buffer = clientSocket->readAll();
            QString message = QString::fromUtf8(buffer).trimmed();
            qDebug() << "Serveur | Reçu (string):" << message
                     << "de" << clientSocket->peerAddress().toString()
                     << ":" << clientSocket->peerPort();

            auto handlers = EventHandlerFactory::createHandlers();
            for (const auto &handler : handlers)
            {
                if (handler->canHandle(message))
                {
                    handler->handle(this, clientSocket, message);
                    break;
                }
            }
        }
        else
        {
            QByteArray buffer = clientSocket->readAll();
            QDataStream stream(buffer);
            stream.setByteOrder(QDataStream::BigEndian);
            stream.setVersion(QDataStream::Qt_6_0);

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

/**
 * @brief Gère la déconnexion d'un client.
 */
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

/**
 * @brief Envoie un message à tous les joueurs.
 * @param message Le message à envoyer.
 */
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

/**
 * @brief Envoie un message à un joueur spécifique.
 * @param playerId L'ID du joueur.
 * @param message Le message à envoyer.
 */
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

/**
 * @brief Envoie un message binaire à un joueur spécifique.
 * @param playerId L'ID du joueur.
 * @param messageType Le type de message.
 * @param payload Les données du message.
 */
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

    stream << messageType;
    data.append(payload);

    clientSocket->write(data);
    clientSocket->flush();
}

/**
 * @brief Envoie un message binaire à tous les joueurs sauf un.
 * @param excludedPlayerId L'ID du joueur à exclure.
 * @param messageType Le type de message.
 * @param payload Les données du message.
 */
void GameServer::sendBinaryToAllExcept(int excludedPlayerId, quint8 messageType, const QByteArray &payload) {
    for (auto playerId : players.keys()) {
        if (playerId != excludedPlayerId) {
            sendBinaryToPlayer(playerId, messageType, payload);
        }
    }
}

/**
 * @brief Diffuse un message binaire à tous les joueurs.
 * @param messageType Le type de message.
 * @param payload Les données du message.
 */
void GameServer::broadcastBinaryData(quint8 messageType, const QByteArray &payload) {
    for (auto playerId : players.keys()) {
        sendBinaryToPlayer(playerId, messageType, payload);
    }
}

/**
 * @brief Envoie la position de la raquette d'un joueur.
 * @param playerId L'ID du joueur.
 * @param paddleY La position Y de la raquette.
 */
void GameServer::sendPaddlePosition(int playerId, float paddleY) {
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setVersion(QDataStream::Qt_6_0);

    stream << static_cast<qint32>(playerId);
    stream << paddleY;

    sendBinaryToAllExcept(playerId, playerId, payload);
}

/**
 * @brief Envoie la position de la balle.
 * @param playerId L'ID du joueur.
 * @param ballY La position Y de la balle.
 * @param ballX La position X de la balle.
 */
void GameServer::sendBallPosition(int playerId, float ballY, float ballX) {
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setVersion(QDataStream::Qt_6_0);

    stream << ballY;
    stream << ballX;

    sendBinaryToAllExcept(playerId, 5, payload);
}

/**
 * @brief Envoie les informations de la salle d'attente à un joueur.
 * @param playerId L'ID du joueur.
 */
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

/**
 * @brief Met à jour les informations de la salle d'attente pour tous les joueurs.
 */
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

/**
 * @brief Vérifie si tous les joueurs sont prêts et démarre le jeu.
 */
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

/**
 * @brief Trouve l'ID d'un joueur à partir de son adresse IP et de son port.
 * @param ip L'adresse IP du joueur.
 * @param port Le port du joueur.
 * @return L'ID du joueur, ou -1 si non trouvé.
 */
int GameServer::findPlayerId(const QHostAddress &ip, quint16 port)
{
    for (auto it = players.begin(); it != players.end(); ++it)
    {
        if (it.value().socket->peerAddress() == ip && it.value().socket->peerPort() == port)
            return it.key();
    }
    return -1;
}