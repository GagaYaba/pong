#include "gameclient.h"
#include <QDebug>
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
    void handle(GameClient* client, const QString &message) override {
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
    void handle(GameClient* client, const QString &message) override {
        QStringList parts = message.split(" ");
        if (parts.size() > 1) {
            client->playerId = parts[1].toInt();
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
    void handle(GameClient* client, const QString &message) override {
        QStringList parts = message.split(" ");
        parts.removeFirst(); // Supprime le mot-clé
        qDebug() << "Slots disponibles:" << parts;
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
    void handle(GameClient* client, const QString &message) override {
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
    void handle(GameClient* client, const QString &message) override {
        QStringList parts = message.split(" ");
        if (parts.size() > 2) {
            int updatedPlayerId = parts[1].toInt();
            QString role = parts[2];
            qDebug() << "Joueur" << updatedPlayerId << "mis à jour avec le rôle:" << role;
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
    void handle(GameClient* client, const QString &message) override {
        Q_UNUSED(message);
        qDebug() << "La partie commence!";
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
    void handle(GameClient* client, const QString &message) override {
        QStringList parts = message.split(" ");
        parts.removeFirst(); // Retire le préfixe "GAME_INFO"
        qDebug() << "Informations de la partie:";
        for (const QString &info : parts) {
            qDebug() << info;
        }
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
    void handle(GameClient* client, const QString &message) override {
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
    void handle(GameClient* client, const QString &message) override {
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
}

void GameClient::connectToServer(const QHostAddress &serverAddr)
{
    this->serverAddress = serverAddr;

    QList<quint16> ports = {27460, 25518, 27718, 28147, 27808, 26897, 29102, 25499, 27520, 27392};
    bool connected = false;

    for (quint16 port : ports) {
        tcpSocket->connectToHost(serverAddr, port);
        if (tcpSocket->waitForConnected(3000)) {
            qDebug() << "Connexion établie avec le serveur sur le port" << port;
            connected = true;
            break;
        } else {
            qDebug() << "Impossible de se connecter au serveur sur le port" << port << ", test suivant...";
        }
    }

    if (!connected) {
        qDebug() << "Erreur: impossible de se connecter à un des ports disponibles du serveur.";
    }
}

void GameClient::sendMessage(const QString &message)
{
    QByteArray data = message.toUtf8();
    if (tcpSocket->state() == QTcpSocket::ConnectedState) {
        tcpSocket->write(data);
        tcpSocket->flush();
        qDebug() << "Message envoyé:" << message;
    } else {
        qDebug() << "Erreur: Connexion non établie";
    }
}

void GameClient::selectRole(const QString &role)
{
    QString message = "SELECT_ROLE " + role;
    sendMessage(message);
    qDebug() << "Demande de sélection de rôle envoyée:" << role;
}

void GameClient::sendPaddlePosition(float paddleY)
{
    if (paddleY != lastPaddleY) { // Envoi uniquement en cas de changement
        lastPaddleY = paddleY;
        QString message = "PADDLE " + QString::number(playerId) + " " + QString::number(paddleY);
        sendMessage(message);
    }
}

void GameClient::onDataReceived()
{
    QByteArray buffer = tcpSocket->readAll();
    QString data = QString::fromUtf8(buffer);
    qDebug() << "CC (" << playerId << ") | Message brut reçu du serveur:" << data;

    // Division des messages reçus sur '\n'
    QStringList messages = data.split("\n", Qt::SkipEmptyParts);

    // Récupération des handlers via la factory
    auto handlers = ClientEventHandlerFactory::createHandlers();

    // Traitement de chaque message avec le handler approprié
    for (const QString &message : messages) {
        qDebug() << "CC (" << playerId << ") | Traitement du message:" << message;
        for (const auto &handler : handlers) {
            if (handler->canHandle(message)) {
                handler->handle(this, message);
                break; // Dès qu'un handler a traité le message, on sort de la boucle
            }
        }
    }
}
