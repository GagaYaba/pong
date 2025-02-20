#ifndef CLIENTEVENTHANDLER_H
#define CLIENTEVENTHANDLER_H

#include <QString>

class GameClient;

class ClientEventHandler {
public:
    virtual ~ClientEventHandler() {}
    virtual bool canHandle(const QString &message) const = 0;
    virtual void handle(GameClient* client, const QString &message) = 0;
};

#endif // CLIENTEVENTHANDLER_H
