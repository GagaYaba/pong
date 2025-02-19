#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);
    void connectToServer(const QHostAddress &serverAddress, quint16 port);
    void sendMessage(const QString &message);
    void selectRole(const QString &role); // Méthode pour envoyer une demande de sélection de rôle
    void sendPaddlePosition(float paddleY); // Méthode pour envoyer la position de la raquette

private slots:
    void onDataReceived();

private:
    QTcpSocket *tcpSocket;   // Remplacer QUdpSocket par QTcpSocket
    int playerId;
    int lastPaddleY;
    QHostAddress serverAddress;
    quint16 serverPort;
};

#endif // GAMECLIENT_H
