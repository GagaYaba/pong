#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QHostAddress>

struct PlayerInfo {
    QTcpSocket* socket;
    QString role;
    bool ready;
};

class GameServer : public QObject {
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);
    void startServer(int mode, bool autoAssign);
    void sendMessageToAll(const QString &message);
    void sendMessageToPlayer(int playerId, const QString &message);
    void sendWaitingRoomInfo(int playerId);
    void updateWaitingRoomForAll();
    void checkAndStartGame();
    int findPlayerId(const QHostAddress &ip, quint16 port);
    int maxPlayers;
    int currentPlayers;
    bool autoAssignRoles;
    QMap<int, PlayerInfo> players;
    QMap<QString, bool> roleTaken;
    QStringList rolesList;
    int gameMode;

private slots:
    void onNewConnection();
    void onDataReceived();
    void onDisconnected();

private:
    QTcpServer* tcpServer;


};

#endif // GAMESERVER_H
