#ifndef BINARYCLIENTEVENTHANDLER_H
#define BINARYCLIENTEVENTHANDLER_H

#include <QString>

class GameClient;

class BinaryClientEventHandler {
public:
    virtual ~BinaryClientEventHandler() {}
    virtual bool canHandle(quint8 msgType) const = 0;
    virtual void handle(GameClient *client, const QByteArray &data) = 0;
};

#endif // CLIENTEVENTHANDLER_H
