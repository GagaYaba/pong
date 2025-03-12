#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);
    void connectToServer(const QHostAddress &serverAddress, quint16 port);
    void sendMessage(const QString &message, const QHostAddress &serverAddress, quint16 port);
    void selectRole(const QString &role);
    void sendPaddlePosition(float paddleY);

private slots:
    void onDataReceived();

private:
    QUdpSocket *udpSocket;
    int playerId;
    int lastPaddleY;
    QHostAddress serverAddress;
    quint16 serverPort;
};

#endif // GAMECLIENT_H
