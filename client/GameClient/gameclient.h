#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <memory>
#include <vector>

#include "ClientEventHandler.h"
#include "ClientEventHandlerFactory.h"
#include "BinaryClientEventHandler.h"
#include "BinaryEventHandlerFactory.h"

class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);

    void connectToServer(const QHostAddress &serverAddr);
    void startGame();
    void sendMessage(const QString &message);

    void selectRole(const QString &role);
    void sendPaddlePositionBinary(float paddleY);
    void simulatePaddleData();
    void checkPaddlePosition();
    void startCheckPaddleTimer();
    QTimer *checkPaddleTimer;

    int playerId;

signals:
    void availableSlotsReceived(const QStringList &slots);
    void roleEmit(const QString &role, int playerId, bool join);
    void gameStartedEmit();
    void gameInfoReceived(const QString &gameMode);


private slots:
    void onDataReceived();


private:
    QTcpSocket *tcpSocket;
    QHostAddress serverAddress;
    float lastPaddleY = -1;
    QTimer *simulationTimer;
    float simulationPaddleY { 0.0f };
    
    std::vector<std::unique_ptr<ClientEventHandler>> handlers;
};

#endif // GAMECLIENT_H
