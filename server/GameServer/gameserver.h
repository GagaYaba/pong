#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QHostAddress>

/**
 * @brief Structure contenant les informations d'un joueur.
 */
struct PlayerInfo {
    QTcpSocket* socket; /**< Le socket du joueur. */
    QString role; /**< Le rôle du joueur. */
    bool ready; /**< Indique si le joueur est prêt. */
};

/**
 * @brief Classe représentant le serveur de jeu.
 */
class GameServer : public QObject {
Q_OBJECT
public:
    /**
     * @brief Constructeur de la classe GameServer.
     * @param parent Le parent QObject.
     */
    explicit GameServer(QObject *parent = nullptr);

    /**
     * @brief Démarre le serveur de jeu.
     * @param mode Le mode de jeu.
     * @param autoAssign Indique si les rôles doivent être assignés automatiquement.
     */
    void startServer(int mode, bool autoAssign);

    /**
     * @brief Envoie un message à tous les joueurs.
     * @param message Le message à envoyer.
     */
    void sendMessageToAll(const QString &message);

    /**
     * @brief Envoie un message à un joueur spécifique.
     * @param playerId L'ID du joueur.
     * @param message Le message à envoyer.
     */
    void sendMessageToPlayer(int playerId, const QString &message);

    /**
     * @brief Envoie des données binaires à tous les joueurs sauf un.
     * @param excludedPlayerId L'ID du joueur à exclure.
     * @param messageType Le type de message.
     * @param payload Les données à envoyer.
     */
    void sendBinaryToAllExcept(int excludedPlayerId, quint8 messageType, const QByteArray &payload);

    /**
     * @brief Envoie des données binaires à un joueur spécifique.
     * @param playerId L'ID du joueur.
     * @param messageType Le type de message.
     * @param payload Les données à envoyer.
     */
    void sendBinaryToPlayer(int playerId, quint8 messageType, const QByteArray &payload);

    /**
     * @brief Diffuse des données binaires à tous les joueurs.
     * @param messageType Le type de message.
     * @param payload Les données à envoyer.
     */
    void broadcastBinaryData(quint8 messageType, const QByteArray &payload);

    /**
     * @brief Envoie la position de la raquette d'un joueur.
     * @param playerId L'ID du joueur.
     * @param paddleY La position Y de la raquette.
     */
    void sendPaddlePosition(int playerId, float paddleY);

    /**
     * @brief Envoie la position de la balle.
     * @param playerId L'ID du joueur.
     * @param ballY La position Y de la balle.
     * @param ballX La position X de la balle.
     */
    void sendBallPosition(int playerId, float ballY, float ballX);

    /**
     * @brief Envoie les informations de la salle d'attente à un joueur.
     * @param playerId L'ID du joueur.
     */
    void sendWaitingRoomInfo(int playerId);

    /**
     * @brief Met à jour les informations de la salle d'attente pour tous les joueurs.
     */
    void updateWaitingRoomForAll();

    /**
     * @brief Vérifie si tous les joueurs sont prêts et démarre le jeu.
     */
    void checkAndStartGame();

    /**
     * @brief Trouve l'ID d'un joueur à partir de son adresse IP et de son port.
     * @param ip L'adresse IP du joueur.
     * @param port Le port du joueur.
     * @return L'ID du joueur, ou -1 si non trouvé.
     */
    int findPlayerId(const QHostAddress &ip, quint16 port);

    int maxPlayers; /**< Le nombre maximum de joueurs. */
    int currentPlayers; /**< Le nombre actuel de joueurs. */
    bool autoAssignRoles; /**< Indique si les rôles doivent être assignés automatiquement. */
    QMap<int, PlayerInfo> players; /**< La liste des joueurs. */
    QMap<QString, bool> roleTaken; /**< Indique si un rôle est pris. */
    QStringList rolesList; /**< La liste des rôles disponibles. */
    int gameMode; /**< Le mode de jeu. */

private slots:
    /**
     * @brief Slot appelé lors d'une nouvelle connexion.
     */
    void onNewConnection();

    /**
     * @brief Slot appelé lors de la réception de données.
     */
    void onDataReceived();

    /**
     * @brief Slot appelé lors de la déconnexion d'un joueur.
     */
    void onDisconnected();

private:
    QTcpServer* tcpServer; /**< Le serveur TCP. */
};

#endif // GAMESERVER_H