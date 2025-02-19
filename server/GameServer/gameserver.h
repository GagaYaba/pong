#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMap>
#include <QHostAddress>
#include <QStringList>

// Structure pour stocker les infos d'un joueur
struct PlayerInfo {
    QTcpSocket *socket;  // Socket du joueur
    QString role;        // Rôle choisi (ex: "p1", "p2", etc.). Vide si non encore attribué.
    bool ready;          // True si le joueur a choisi son rôle
};

class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);
    // mode: 1 pour 1vs1, 2 pour 2vs2 ; autoAssign permet de choisir l'attribution automatique ou non des rôles
    void startServer(int mode, bool autoAssign = false);
    void sendMessageToAll(const QString &message);
    void sendMessageToPlayer(int playerId, const QString &message);

private slots:
    void onNewConnection();
    void onDataReceived();
    void onDisconnected();

private:
    QTcpServer *tcpServer;
    QMap<int, PlayerInfo> players; // clé = id du joueur
    int maxPlayers;
    int currentPlayers;
    int gameMode; // 1 pour 1vs1, 2 pour 2vs2
    bool autoAssignRoles;

    QStringList rolesList;           // liste des rôles disponibles (ex: {"p1", "p2"} ou {"p1", "p2", "p3", "p4"})
    QMap<QString, bool> roleTaken;   // indique si un rôle est déjà pris

    void sendWaitingRoomInfo(int playerId);
    void updateWaitingRoomForAll();
    void checkAndStartGame();
    int findPlayerId(const QHostAddress &ip, quint16 port);
};

#endif // GAMESERVER_H
