#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QMap>
#include <QHostAddress>
#include <QStringList>

struct PlayerInfo {
    QHostAddress ip;
    quint16 port;
    QString role;
    bool ready;
};

class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);
    void startServer(quint16 port, int mode, bool autoAssign = false);
    void sendMessageToAll(const QString &message);
    void sendMessageToPlayer(int playerId, const QString &message);

private slots:
    void onDataReceived();

private:
    QUdpSocket *udpSocket;
    QMap<int, PlayerInfo> players;
    int maxPlayers;
    int currentPlayers;
    int gameMode;
    bool autoAssignRoles;

    QStringList rolesList;
    QMap<QString, bool> roleTaken;

    void sendWaitingRoomInfo(int playerId);
    void updateWaitingRoomForAll();
    void checkAndStartGame();
    int findPlayerId(const QHostAddress &ip, quint16 port);
};

#endif // GAMESERVER_H
