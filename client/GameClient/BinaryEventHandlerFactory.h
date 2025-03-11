#ifndef BINARYEVENTHANDLERFACTORY_H
#define BINARYEVENTHANDLERFACTORY_H

#include <QString>

class GameClient;

class BinaryEventHandlerFactory {
public:
    static std::vector<std::unique_ptr<BinaryClientEventHandler>> createHandlers();

};


#endif // CLIENTEVENTHANDLER_H
