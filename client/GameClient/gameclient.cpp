#include "gameclient.h"
#include "../../include/SelectDialog.h"
#include "../../include/globals.h"
#include "../../include/game.h"
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QTcpSocket>
#include <memory>
#include <vector>

// =====================================================
// Handler pour "ROLE_ASSIGNED"
// =====================================================
class RoleAssignedEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("ROLE_ASSIGNED");
    }

    void handle(GameClient *client, const QString &message) override {
        qDebug() << "Rôle attribué";
        QStringList parts = message.split(" ");
        if (parts.size() > 1) {
            QString role = parts[1];
            qDebug() << "Rôle attribué:" << role;
        } else {
            qDebug() << "Erreur: message ROLE_ASSIGNED mal formé";
        }
    }
};

// =====================================================
// Handler pour "ASSIGN_ID"
// =====================================================
class AssignIdEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("ASSIGN_ID");
    }

    void handle(GameClient *client, const QString &message) override {
        QStringList parts = message.split(" ");
        if (parts.size() > 1) {
            client->playerId = parts[1].toInt();
            g_playerId = client->playerId;
            qDebug() << "ID attribué:" << client->playerId;
        } else {
            qDebug() << "Erreur: message ASSIGN_ID mal formé";
        }
    }
};

// =====================================================
// Handler pour "AVAILABLE_SLOTS"
// =====================================================
class AvailableSlotsEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("AVAILABLE_SLOTS");
    }

    void handle(GameClient *client, const QString &message) override {
        QStringList parts = message.split(" ");  // utilise QStringList et non QList<QString>
        parts.removeFirst();
        if (client) {
            qDebug() << "Emission du signal availableSlotsReceived avec" << parts;
            emit client->availableSlotsReceived(parts);
        } else {
            qDebug() << "Erreur: GameClient est NULL";
        }

    }
};

// =====================================================
// Handler pour "PLAYER_JOINED"
// =====================================================
class PlayerJoinedEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("PLAYER_JOINED");
    }

    void handle(GameClient *client, const QString &message) override {
        QStringList parts = message.split(" ");
        if (parts.size() > 1) {
            int joinedPlayerId = parts[1].toInt();
            qDebug() << "Nouveau joueur rejoint:" << joinedPlayerId;
        }
    }
};

// =====================================================
// Handler pour "PLAYER_UPDATED"
// =====================================================
class PlayerUpdatedEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("PLAYER_UPDATED");
    }

    void handle(GameClient *client, const QString &message) override {
        QStringList parts = message.split(" ");
        if (parts.size() > 2) {
            int updatedPlayerId = parts[1].toInt();
            QString role = parts[2];

            if (updatedPlayerId == client->playerId) {
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
class GameInfoEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("GAME_INFO");
    }

    void handle(GameClient *client, const QString &message) override {
        QStringList parts = message.split(" ");
        if (parts.size() > 1) {
            QString gameMode = parts[1];
            g_gameMode = gameMode;
            qDebug() << "Mode de jeu:" << gameMode;
            QString message = "READY " + QString::number(client->playerId);
            client->sendMessage(message);
            emit client->gameInfoReceived(parts[1]); // Émet le signal pour fermer le dialog
        } else {
            qDebug() << "Erreur: message GAME_INFO mal formé";
        }
    }
};

// =====================================================
// Handler pour "GAME_START"
// =====================================================
class GameStartEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("GAME_START");
    }

    void handle(GameClient *client, const QString &message) override {
        Q_UNUSED(message);
        qDebug() << "La partie commence!";
        g_game = new Game;
        g_game->show();
        client->startCheckPaddleTimer();
    }
};

// =====================================================
// Handler pour "FULL"
// =====================================================
class FullEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        return message.startsWith("FULL");
    }

    void handle(GameClient *client, const QString &message) override {
        Q_UNUSED(message);
        qDebug() << "Serveur complet, impossible de se connecter";
    }
};

// =====================================================
// Handler par défaut pour les messages inconnus
// =====================================================
class UnknownEventHandler : public ClientEventHandler {
public:
    bool canHandle(const QString &message) const override {
        // Toujours vrai en fallback
        return true;
    }

    void handle(GameClient *client, const QString &message) override {
        qDebug() << "Message inconnu reçu:" << message;
    }
};

// =====================================================
// Factory pour créer les handlers côté client
// =====================================================
std::vector<std::unique_ptr<ClientEventHandler>> ClientEventHandlerFactory::createHandlers() {
    std::vector<std::unique_ptr<ClientEventHandler>> handlers;

    // Ajout des gestionnaires d'événements spécifiques
    handlers.push_back(std::make_unique<RoleAssignedEventHandler>());
    handlers.push_back(std::make_unique<AssignIdEventHandler>());
    handlers.push_back(std::make_unique<AvailableSlotsEventHandler>());
    handlers.push_back(std::make_unique<PlayerJoinedEventHandler>());
    handlers.push_back(std::make_unique<PlayerUpdatedEventHandler>());
    handlers.push_back(std::make_unique<GameStartEventHandler>());
    handlers.push_back(std::make_unique<GameInfoEventHandler>());
    handlers.push_back(std::make_unique<FullEventHandler>());
    handlers.push_back(std::make_unique<UnknownEventHandler>());  // Handler par défaut

    return handlers;
}

// =====================================================
// Implémentation de GameClient
// =====================================================
GameClient::GameClient(QObject *parent)
    : QObject(parent),
      tcpSocket(new QTcpSocket(this)),
      playerId(-1)
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &GameClient::onDataReceived);

    // simulationTimer = new QTimer(this);
    // connect(simulationTimer, &QTimer::timeout, this, &GameClient::simulatePaddleData);
    // simulationTimer->start(5);
}

void GameClient::startCheckPaddleTimer() {
    checkPaddleTimer = new QTimer(this);
    connect(checkPaddleTimer, &QTimer::timeout, this, &GameClient::checkPaddlePosition);
    checkPaddleTimer->start(10);
}

void GameClient::simulatePaddleData() {
    simulationPaddleY += 5.0f;
    if (simulationPaddleY > 500.0f)
        simulationPaddleY = 0.0f;
    sendPaddlePositionBinary(simulationPaddleY);
}

void GameClient::checkPaddlePosition() {
    if (g_game) {
        float paddleY = g_game->players[0]->getPaddle()->y();
        sendPaddlePositionBinary(paddleY);
    }
}

// void GameClient::checkPaddlePosition() {
//     lastPaddleY = -1;

//     qDebug() << g_playerId;


//     if (!g_game) {
//         qDebug() << "Erreur: g_game est NULL";
//         return;
//     }

//     if (g_playerId < 0 || g_playerId >= g_game->players.size()) {
//         qDebug() << "Erreur: g_playerId (" << g_playerId << ") hors limites";
//         return;
//     }

//     Player* player = g_game->players[g_playerId];  // Récupérer le joueur
//     if (!player) {
//         qDebug() << "Erreur: Player est NULL";
//         return;
//     }

//     Paddle* paddle = player->getPaddle();  // Récupérer le paddle
//     if (!paddle) {
//         qDebug() << "Erreur: Paddle est NULL";
//         return;
//     }

//     float paddleY = paddle->y();
//     if (paddleY != lastPaddleY) {
//         lastPaddleY = paddleY;
//         sendPaddlePositionBinary(paddleY);
//     }
// }

void GameClient::sendPaddlePositionBinary(float paddleY) {
    // Vérifier si la position a changé avant d'envoyer
    if (paddleY != lastPaddleY) {
        qDebug() << "Client | Position du paddle:" << paddleY;
        lastPaddleY = paddleY;

        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian); // Format réseau
        stream.setVersion(QDataStream::Qt_6_0);

        quint8 messageType = 1; // Identifiant de message "Paddle Move"
        stream << messageType;
        stream << static_cast<qint32>(playerId); // ID du joueur
        stream << paddleY; // Position Y du paddle

        // Vérifier si le socket est bien connecté avant d'envoyer
        if (tcpSocket->state() == QTcpSocket::ConnectedState) {
            tcpSocket->write(data);
            tcpSocket->flush();
            qDebug() << "Client | Message binaire envoyé:" << data.toHex();
        } else {
            qDebug() << "Client | Erreur: Connexion non établie";
        }
    }
}

void GameClient::sendMessage(const QString &message) {
    QString messageToSend = message + "\n";
    QByteArray data = messageToSend.toUtf8();
    if (tcpSocket->state() == QTcpSocket::ConnectedState) {
        tcpSocket->write(data);
        tcpSocket->flush();
        qDebug() << "Message envoyé:" << messageToSend;
    } else {
        qDebug() << "Erreur: Connexion non établie";
    }
}

void GameClient::connectToServer(const QHostAddress &serverAddr) {
    this->serverAddress = serverAddr;

    QList<quint16> ports = {27460, 25518, 27718, 28147, 27808, 26897, 29102, 25499, 27520, 27392};
    bool connected = false;

    for (quint16 port: ports) {
        tcpSocket->connectToHost(serverAddr, port);
        if (tcpSocket->waitForConnected(3000)) {
            qDebug() << "Connexion établie avec le serveur sur le port" << port;
            connected = true;
            QString message = "JOIN";
            sendMessage(message);
            break;
        } else {
            qDebug() << "Impossible de se connecter au serveur sur le port" << port << ", test suivant...";
        }
    }

    if (!connected) {
        qDebug() << "Erreur: impossible de se connecter à un des ports disponibles du serveur.";
    }
}

void GameClient::selectRole(const QString &role) {
    QString message = "SELECT_ROLE " + role;
    sendMessage(message);
    qDebug() << "Demande de sélection de rôle envoyée:" << role;
}

void GameClient::startGame() {
    QString message = "START_GAME";
    sendMessage(message);
    qDebug() << "Demande de démarrage de la partie envoyée";
}

void GameClient::onDataReceived() {
    QByteArray buffer = tcpSocket->readAll();
    QString data = QString::fromUtf8(buffer);
    qDebug() << "CC (" << playerId << ") | Message brut reçu du serveur:" << data;

    // Division des messages reçus sur '\n'
    QStringList messages = data.split("\n", Qt::SkipEmptyParts);

    // Récupération des handlers via la factory
    auto handlers = ClientEventHandlerFactory::createHandlers();

    // Traitement de chaque message avec le handler approprié
    for (const QString &message: messages) {
        qDebug() << "CC (" << playerId << ") | Traitement du message:" << message;
        for (const auto &handler: handlers) {
            if (handler->canHandle(message)) {
                handler->handle(this, message);
                break; // Dès qu'un handler a traité le message, on sort de la boucle
            }
        }
    }
}
