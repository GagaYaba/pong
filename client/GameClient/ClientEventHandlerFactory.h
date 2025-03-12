#ifndef CLIENTEVENTHANDLERFACTORY_H
#define CLIENTEVENTHANDLERFACTORY_H

#include <memory>
#include <vector>

/**
 * @brief Factory pour créer les gestionnaires d'événements côté client.
 */
class ClientEventHandlerFactory {
public:
    /**
     * @brief Crée une liste de gestionnaires d'événements côté client.
     * @return Un vecteur de gestionnaires d'événements côté client.
     */
    static std::vector<std::unique_ptr<ClientEventHandler>> createHandlers();
};

#endif // CLIENTEVENTHANDLERFACTORY_H