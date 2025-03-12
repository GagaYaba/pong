#include "gameclient.h"
#include "../../include/SelectDialog.h"
#include "../../include/globals.h"
#include "../../include/game.h"
#include "BinaryClientEventHandler.h"
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QTcpSocket>
#include <memory>
#include <vector>

// =====================================================
// Handler pour "ROLE_ASSIGNED"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "ROLE_ASSIGNED".
 */
class RoleAssignedEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "ROLE_ASSIGNED".
     * @param message Le message à vérifier.
     * @return true si le message commence par "ROLE_ASSIGNED", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("ROLE_ASSIGNED");
    }

    /**
     * @brief Traite le message "ROLE_ASSIGNED".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        qDebug() << "Rôle attribué";
        QStringList parts = message.split(" ");
        if (parts.size() > 1)
        {
            QString role = parts[1];
            qDebug() << "Rôle attribué:" << role;
        }
        else
        {
            qDebug() << "Erreur: message ROLE_ASSIGNED mal formé";
        }
    }
};

// =====================================================
// Handler pour "ASSIGN_ID"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "ASSIGN_ID".
 */
class AssignIdEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "ASSIGN_ID".
     * @param message Le message à vérifier.
     * @return true si le message commence par "ASSIGN_ID", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("ASSIGN_ID");
    }

    /**
     * @brief Traite le message "ASSIGN_ID".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        QStringList parts = message.split(" ");
        if (parts.size() > 1)
        {
            client->playerId = parts[1].toInt();
            g_playerId = client->playerId;
            qDebug() << "ID attribué:" << client->playerId;
        }
        else
        {
            qDebug() << "Erreur: message ASSIGN_ID mal formé";
        }
    }
};

// =====================================================
// Handler pour "AVAILABLE_SLOTS"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "AVAILABLE_SLOTS".
 */
class AvailableSlotsEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "AVAILABLE_SLOTS".
     * @param message Le message à vérifier.
     * @return true si le message commence par "AVAILABLE_SLOTS", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("AVAILABLE_SLOTS");
    }

    /**
     * @brief Traite le message "AVAILABLE_SLOTS".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        QStringList parts = message.split(" ");
        parts.removeFirst();
        if (client)
        {
            qDebug() << "Emission du signal availableSlotsReceived avec" << parts;
            emit client->availableSlotsReceived(parts);
        }
        else
        {
            qDebug() << "Erreur: GameClient est NULL";
        }
    }
};

// =====================================================
// Handler pour "PLAYER_JOINED"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "PLAYER_JOINED".
 */
class PlayerJoinedEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "PLAYER_JOINED".
     * @param message Le message à vérifier.
     * @return true si le message commence par "PLAYER_JOINED", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("PLAYER_JOINED");
    }

    /**
     * @brief Traite le message "PLAYER_JOINED".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        QStringList parts = message.split(" ");
        if (parts.size() > 1)
        {
            int joinedPlayerId = parts[1].toInt();
            qDebug() << "Nouveau joueur rejoint:" << joinedPlayerId;
        }
    }
};

// =====================================================
// Handler pour "PLAYER_UPDATED"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "PLAYER_UPDATED".
 */
class PlayerUpdatedEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "PLAYER_UPDATED".
     * @param message Le message à vérifier.
     * @return true si le message commence par "PLAYER_UPDATED", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("PLAYER_UPDATED");
    }

    /**
     * @brief Traite le message "PLAYER_UPDATED".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        QStringList parts = message.split(" ");
        if (parts.size() > 2)
        {
            int updatedPlayerId = parts[1].toInt();
            QString role = parts[2];

            if (updatedPlayerId == client->playerId)
            {
                g_playerRole = role;
            }

            emit client->roleEmit(role, updatedPlayerId, true);

            qDebug() << "Joueur" << updatedPlayerId << "mis à jour avec le rôle:" << role;
        }
    }
};

// =====================================================
// Handler pour "GAME_INFO"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "GAME_INFO".
 */
class GameInfoEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "GAME_INFO".
     * @param message Le message à vérifier.
     * @return true si le message commence par "GAME_INFO", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("GAME_INFO");
    }

    /**
     * @brief Traite le message "GAME_INFO".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        QStringList parts = message.split(" ");
        if (parts.size() > 1)
        {
            QString gameMode = parts[1];
            g_gameMode = gameMode;
            qDebug() << "Mode de jeu:" << gameMode;
            QString message = "READY " + QString::number(client->playerId);
            client->sendMessage(message);
            emit client->gameInfoReceived(parts[1]);
        }
        else
        {
            qDebug() << "Erreur: message GAME_INFO mal formé";
        }
    }
};

// =====================================================
// Handler pour "GAME_START"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "GAME_START".
 */
class GameStartEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "GAME_START".
     * @param message Le message à vérifier.
     * @return true si le message commence par "GAME_START", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("GAME_START");
    }

    /**
     * @brief Traite le message "GAME_START".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        Q_UNUSED(message);
        qDebug() << "La partie commence!";
        g_game = new Game;
        g_game->show();
        if (g_isHost)
        {
            client->startCheckBallTimer();
        }
        client->startCheckPaddleTimer();
    }
};

// =====================================================
// Handler pour "FULL"
// =====================================================
/**
 * @brief Gestionnaire d'événements pour le message "FULL".
 */
class FullEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message "FULL".
     * @param message Le message à vérifier.
     * @return true si le message commence par "FULL", false sinon.
     */
    bool canHandle(const QString &message) const override
    {
        return message.startsWith("FULL");
    }

    /**
     * @brief Traite le message "FULL".
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        Q_UNUSED(message);
        qDebug() << "Serveur complet, impossible de se connecter";
    }
};

// =====================================================
// Handler par défaut pour les messages inconnus
// =====================================================
/**
 * @brief Gestionnaire d'événements par défaut pour les messages inconnus.
 */
class UnknownEventHandler : public ClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message inconnu.
     * @param message Le message à vérifier.
     * @return true.
     */
    bool canHandle(const QString &message) const override
    {
        return true;
    }

    /**
     * @brief Traite le message inconnu.
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    void handle(GameClient *client, const QString &message) override
    {
        qDebug() << "Message inconnu reçu:" << message;
    }
};

// =====================================================
// Factory pour créer les handlers côté client
// =====================================================
/**
 * @brief Crée une liste de gestionnaires d'événements côté client.
 * @return Un vecteur de gestionnaires d'événements côté client.
 */
std::vector<std::unique_ptr<ClientEventHandler>> ClientEventHandlerFactory::createHandlers()
{
    std::vector<std::unique_ptr<ClientEventHandler>> handlers;

    handlers.push_back(std::make_unique<RoleAssignedEventHandler>());
    handlers.push_back(std::make_unique<AssignIdEventHandler>());
    handlers.push_back(std::make_unique<AvailableSlotsEventHandler>());
    handlers.push_back(std::make_unique<PlayerJoinedEventHandler>());
    handlers.push_back(std::make_unique<PlayerUpdatedEventHandler>());
    handlers.push_back(std::make_unique<GameStartEventHandler>());
    handlers.push_back(std::make_unique<GameInfoEventHandler>());
    handlers.push_back(std::make_unique<FullEventHandler>());
    handlers.push_back(std::make_unique<UnknownEventHandler>());

    return handlers;
}

// =====================================================
// Handler pour "BINARY_PLAYER1"
// =====================================================
/**
 * @brief Gestionnaire d'événements binaires pour le message de type 1.
 */
class BinaryPlayer1EventHandler : public BinaryClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message binaire de type 1.
     * @param msgType Le type de message à vérifier.
     * @return true si le type de message est 1, false sinon.
     */
    bool canHandle(quint8 msgType) const override
    {
        return msgType == 1;
    }

    /**
     * @brief Traite le message binaire de type 1.
     * @param client Le client de jeu.
     * @param data Les données reçues.
     */
    void handle(GameClient *client, const QByteArray &data) override
    {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 type;
        stream >> type;
        qint32 senderId;
        stream >> senderId;
        float paddleY;
        stream >> paddleY;

        int id = senderId;
        PlayerNetwork *player = g_game->playersNetwork[0];
        player->updatePaddlePosition(paddleY);
    }
};

// =====================================================
// Handler pour "BINARY_PLAYER2"
// =====================================================
/**
 * @brief Gestionnaire d'événements binaires pour le message de type 2.
 */
class BinaryPlayer2EventHandler : public BinaryClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message binaire de type 2.
     * @param msgType Le type de message à vérifier.
     * @return true si le type de message est 2, false sinon.
     */
    bool canHandle(quint8 msgType) const override
    {
        return msgType == 2;
    }

    /**
     * @brief Traite le message binaire de type 2.
     * @param client Le client de jeu.
     * @param data Les données reçues.
     */
    void handle(GameClient *client, const QByteArray &data) override
    {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 type;
        stream >> type;
        qint32 senderId;
        stream >> senderId;
        float paddleY;
        stream >> paddleY;

        int id = senderId;
        PlayerNetwork *player = g_game->playersNetwork[0];
        player->updatePaddlePosition(paddleY);
    }
};

// =====================================================
// Handler pour "BINARY_PLAYER3"
// =====================================================
/**
 * @brief Gestionnaire d'événements binaires pour le message de type 3.
 */
class BinaryPlayer3EventHandler : public BinaryClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message binaire de type 3.
     * @param msgType Le type de message à vérifier.
     * @return true si le type de message est 3, false sinon.
     */
    bool canHandle(quint8 msgType) const override
    {
        return msgType == 3;
    }

    /**
     * @brief Traite le message binaire de type 3.
     * @param client Le client de jeu.
     * @param data Les données reçues.
     */
    void handle(GameClient *client, const QByteArray &data) override
    {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 type;
        stream >> type;
        qint32 senderId;
        stream >> senderId;
        float paddleY;
        stream >> paddleY;

        qDebug() << "Handler binaire joueur 3 | ID:" << senderId << "Paddle Y:" << paddleY;
    }
};

// =====================================================
// Handler pour "BINARY_PLAYER4"
// =====================================================
/**
 * @brief Gestionnaire d'événements binaires pour le message de type 4.
 */
class BinaryPlayer4EventHandler : public BinaryClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message binaire de type 4.
     * @param msgType Le type de message à vérifier.
     * @return true si le type de message est 4, false sinon.
     */
    bool canHandle(quint8 msgType) const override
    {
        return msgType == 4;
    }

    /**
     * @brief Traite le message binaire de type 4.
     * @param client Le client de jeu.
     * @param data Les données reçues.
     */
    void handle(GameClient *client, const QByteArray &data) override
    {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 type;
        stream >> type;
        qint32 senderId;
        stream >> senderId;
        float paddleY;
        stream >> paddleY;
    }
};

// =====================================================
// Handler pour "BINARY_BALL"
// =====================================================
/**
 * @brief Gestionnaire d'événements binaires pour le message de type 5.
 */
class BinaryBall5EventHandler : public BinaryClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message binaire de type 5.
     * @param msgType Le type de message à vérifier.
     * @return true si le type de message est 5, false sinon.
     */
    bool canHandle(quint8 msgType) const override
    {
        return msgType == 5;
    }

    /**
     * @brief Traite le message binaire de type 5.
     * @param client Le client de jeu.
     * @param data Les données reçues.
     */
    void handle(GameClient *client, const QByteArray &data) override
    {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 type;
        stream >> type;
        float ballY;
        stream >> ballY;
        float ballX;
        stream >> ballX;

        if (!g_isHost) {
            g_game->getBall()->setPos(ballX, ballY);
        }
    }
};

// =====================================================
// Handler par défaut pour les messages binaires inconnus
// =====================================================
/**
 * @brief Gestionnaire d'événements par défaut pour les messages binaires inconnus.
 */
class BinaryUnknownEventHandler : public BinaryClientEventHandler
{
public:
    /**
     * @brief Vérifie si le gestionnaire peut traiter le message binaire inconnu.
     * @param msgType Le type de message à vérifier.
     * @return true.
     */
    bool canHandle(quint8 msgType) const override
    {
        return true;
    }

    /**
     * @brief Traite le message binaire inconnu.
     * @param client Le client de jeu.
     * @param data Les données reçues.
     */
    void handle(GameClient *client, const QByteArray &data) override
    {
        qDebug() << "Message binaire inconnu reçu:" << data.toHex();
    }
};

// =====================================================
// Factory pour créer les handlers binaires côté client
// =====================================================
/**
 * @brief Crée une liste de gestionnaires d'événements binaires côté client.
 * @return Un vecteur de gestionnaires d'événements binaires côté client.
 */
std::vector<std::unique_ptr<BinaryClientEventHandler>> BinaryEventHandlerFactory::createHandlers()
{
    std::vector<std::unique_ptr<BinaryClientEventHandler>> handlers;

    handlers.push_back(std::make_unique<BinaryPlayer1EventHandler>());
    handlers.push_back(std::make_unique<BinaryPlayer2EventHandler>());
    handlers.push_back(std::make_unique<BinaryPlayer3EventHandler>());
    handlers.push_back(std::make_unique<BinaryPlayer4EventHandler>());
    handlers.push_back(std::make_unique<BinaryBall5EventHandler>());
    handlers.push_back(std::make_unique<BinaryUnknownEventHandler>());

    return handlers;
}

// =====================================================
// Implémentation de GameClient
// =====================================================
/**
 * @brief Constructeur de la classe GameClient.
 * @param parent Le parent QObject.
 */
GameClient::GameClient(QObject *parent)
        : QObject(parent),
          tcpSocket(new QTcpSocket(this)),
          playerId(-1)
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &GameClient::onDataReceived);
}

/**
 * @brief Démarre le timer pour vérifier la position de la raquette.
 */
void GameClient::startCheckPaddleTimer()
{
    checkPaddleTimer = new QTimer(this);
    connect(checkPaddleTimer, &QTimer::timeout, this, &GameClient::checkPaddlePosition);
    checkPaddleTimer->start(5);
}

/**
 * @brief Démarre le timer pour vérifier la position de la balle.
 */
void GameClient::startCheckBallTimer()
{
    checkBallTimer = new QTimer(this);
    connect(checkBallTimer, &QTimer::timeout, this, &GameClient::checkBallPosition);
    checkBallTimer->start(5);
}

/**
 * @brief Simule les données de la raquette.
 */
void GameClient::simulatePaddleData()
{
    simulationPaddleY += 5.0f;
    if (simulationPaddleY > 500.0f)
        simulationPaddleY = 0.0f;
    sendPaddlePositionBinary(simulationPaddleY);
}

/**
 * @brief Vérifie la position de la raquette et envoie les données binaires.
 */
void GameClient::checkPaddlePosition()
{
    if (g_game)
    {
        float paddleY = g_game->players[0]->getPaddle()->y();
        sendPaddlePositionBinary(paddleY);
    }
}

/**
 * @brief Vérifie la position de la balle et envoie les données binaires.
 */
void GameClient::checkBallPosition()
{
    if (g_game)
    {
        float ballY = g_game->getBall()->y();
        float ballX = g_game->getBall()->x();
        sendBallPositionBinary(ballY, ballX);
    }
}

/**
 * @brief Envoie la position de la raquette en données binaires.
 * @param paddleY La position Y de la raquette.
 */
void GameClient::sendPaddlePositionBinary(float paddleY)
{
    if (paddleY != lastPaddleY)
    {
        lastPaddleY = paddleY;

        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 messageType = 1;
        stream << messageType;
        stream << static_cast<qint32>(playerId);
        stream << paddleY;
        if (tcpSocket->state() == QTcpSocket::ConnectedState)
        {
            tcpSocket->write(data);
            tcpSocket->flush();
        }
        else
        {
            qDebug() << "Client | Erreur: Connexion non établie";
        }
    }
}

/**
 * @brief Envoie la position de la balle en données binaires.
 * @param ballY La position Y de la balle.
 * @param ballX La position X de la balle.
 */
void GameClient::sendBallPositionBinary(float ballY, float ballX)
{
    if (ballY != lastBallY || ballX != lastBallX)
    {
        lastBallY = ballY;
        lastBallX = ballX;

        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 messageType = 2;
        stream << messageType;
        stream << static_cast<qint32>(playerId);
        stream << ballY;
        stream << ballX;

        if (tcpSocket->state() == QTcpSocket::ConnectedState)
        {
            tcpSocket->write(data);
            tcpSocket->flush();
        }
        else
        {
            qDebug() << "Client | Erreur: Connexion non établie";
        }
    }
}

/**
 * @brief Envoie un message au serveur.
 * @param message Le message à envoyer.
 */
void GameClient::sendMessage(const QString &message)
{
    QString messageToSend = message + "\n";
    QByteArray data = messageToSend.toUtf8();
    if (tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        tcpSocket->write(data);
        tcpSocket->flush();
        qDebug() << "Message envoyé:" << messageToSend;
    }
    else
    {
        qDebug() << "Erreur: Connexion non établie";
    }
}

/**
 * @brief Se connecte au serveur.
 * @param serverAddr L'adresse du serveur.
 */
void GameClient::connectToServer(const QHostAddress &serverAddr)
{
    this->serverAddress = serverAddr;

    QList<quint16> ports = {27460, 25518, 27718, 28147, 27808, 26897, 29102, 25499, 27520, 27392};
    bool connected = false;

    for (quint16 port : ports)
    {
        tcpSocket->connectToHost(serverAddr, port);
        if (tcpSocket->waitForConnected(3000))
        {
            qDebug() << "Connexion établie avec le serveur sur le port" << port;
            connected = true;
            QString message = "JOIN";
            sendMessage(message);
            break;
        }
        else
        {
            qDebug() << "Impossible de se connecter au serveur sur le port" << port << ", test suivant...";
        }
    }

    if (!connected)
    {
        qDebug() << "Erreur: impossible de se connecter à un des ports disponibles du serveur.";
    }
}

/**
 * @brief Sélectionne un rôle pour le joueur.
 * @param role Le rôle à sélectionner.
 */
void GameClient::selectRole(const QString &role)
{
    QString message = "SELECT_ROLE " + role;
    sendMessage(message);
    qDebug() << "Demande de sélection de rôle envoyée:" << role;
}

/**
 * @brief Démarre la partie.
 */
void GameClient::startGame()
{
    QString message = "START_GAME";
    sendMessage(message);
    qDebug() << "Demande de démarrage de la partie envoyée";
}

/**
 * @brief Gère les données reçues du serveur.
 */
void GameClient::onDataReceived()
{
    QByteArray peekBuffer = tcpSocket->peek(tcpSocket->bytesAvailable());

    if (peekBuffer.endsWith('\n'))
    {
        QByteArray data = tcpSocket->readAll();
        QString message = QString::fromUtf8(data).trimmed();
        qDebug() << "Client (" << playerId << ") | Message string reçu:" << message;

        auto handlers = ClientEventHandlerFactory::createHandlers();
        QStringList messages = message.split("\n", Qt::SkipEmptyParts);
        for (const QString &msg : messages)
        {
            qDebug() << "Client (" << playerId << ") | Traitement du message:" << msg;
            for (const auto &handler : handlers)
            {
                if (handler->canHandle(msg))
                {
                    handler->handle(this, msg);
                    break;
                }
            }
        }
    }
    else
    {
        QByteArray data = tcpSocket->readAll();

        if (data.size() < 1)
        {
            qDebug() << "Client | Message binaire vide ou incorrect";
            return;
        }
        quint8 msgType = static_cast<quint8>(data.at(0));

        auto binaryHandlers = BinaryEventHandlerFactory::createHandlers();
        bool handled = false;
        for (const auto &handler : binaryHandlers)
        {
            if (handler->canHandle(msgType))
            {
                handler->handle(this, data);
                handled = true;
                break;
            }
        }
        if (!handled)
        {
            qDebug() << "Client | Message binaire non reconnu";
        }
    }
}