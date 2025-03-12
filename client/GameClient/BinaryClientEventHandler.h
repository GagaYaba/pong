#ifndef BINARYCLIENTEVENTHANDLER_H
#define BINARYCLIENTEVENTHANDLER_H

#include <QString>

/**
 * @brief Classe de base pour les gestionnaires d'événements binaires côté client.
 */
class GameClient;

/**
 * @brief Interface pour les gestionnaires d'événements binaires côté client.
 */
class BinaryClientEventHandler {
public:
    virtual ~BinaryClientEventHandler() {}

    /**
     * @brief Vérifie si le gestionnaire peut traiter le type de message donné.
     * @param msgType Le type de message à vérifier.
     * @return true si le gestionnaire peut traiter le type de message, false sinon.
     */
    virtual bool canHandle(quint8 msgType) const = 0;

    /**
     * @brief Traite les données reçues du serveur.
     * @param client Le client de jeu.
     * @param data Les données reçues.
     */
    virtual void handle(GameClient *client, const QByteArray &data) = 0;
};

#endif // CLIENTEVENTHANDLER_H