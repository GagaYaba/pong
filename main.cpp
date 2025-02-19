#include <QApplication>
#include <QDebug>
#include "Game.h"

#include "server/GameServer/gameserver.h"
#include "client/GameClient/gameclient.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto server = new GameServer(&a);
    server->startServer(12345, 1, false);

    auto client = new GameClient(&a);
    // client->connectToServer(QHostAddress("10.31.34.229"), 12345);
    client->connectToServer(QHostAddress::LocalHost, 12345);

    // QTimer::singleShot(3000, [=](){
    //     client->selectRole("p2");
    // });

    Game game;
    game.show();
    return a.exec();
}
