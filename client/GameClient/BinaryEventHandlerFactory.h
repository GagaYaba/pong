#ifndef BINARYEVENTHANDLERFACTORY_H
#define BINARYEVENTHANDLERFACTORY_H

#include <QString>

/**
 * @brief Classe de base pour les gestionnaires d'événements binaires côté client.
 */
class GameClient;

/**
 * @brief Factory pour créer les gestionnaires d'événements binaires côté client.
 */
class BinaryEventHandlerFactory {
public:
    /**
     * @brief Crée une liste de gestionnaires d'événements binaires.
     * @return Un vecteur de gestionnaires d'événements binaires.
     */
    static std::vector<std::unique_ptr<BinaryClientEventHandler>> createHandlers();
};

#endif // CLIENTEVENTHANDLER_H