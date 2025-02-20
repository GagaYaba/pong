#ifndef CLIENTEVENTHANDLERFACTORY_H
#define CLIENTEVENTHANDLERFACTORY_H

#include <memory>
#include <vector>

class ClientEventHandlerFactory {
public:
    static std::vector<std::unique_ptr<ClientEventHandler>> createHandlers();
};

#endif // CLIENTEVENTHANDLERFACTORY_H