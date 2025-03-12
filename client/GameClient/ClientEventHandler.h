#ifndef CLIENTEVENTHANDLER_H
#define CLIENTEVENTHANDLER_H

#include <QString>

/**
 * @brief Classe de base pour les gestionnaires d'événements côté client.
 */
class GameClient;

/**
 * @brief Interface pour les gestionnaires d'événements côté client.
 */
class ClientEventHandler {
public:
    virtual ~ClientEventHandler() {}

    /**
     * @brief Vérifie si le gestionnaire peut traiter le message donné.
     * @param message Le message à vérifier.
     * @return true si le gestionnaire peut traiter le message, false sinon.
     */
    virtual bool canHandle(const QString &message) const = 0;

    /**
     * @brief Traite le message reçu du serveur.
     * @param client Le client de jeu.
     * @param message Le message reçu.
     */
    virtual void handle(GameClient* client, const QString &message) = 0;
};

#endif // CLIENTEVENTHANDLER_H