#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QHostAddress>

class GameClient;
class Game;

/**
 * @brief Pointeur global vers l'objet GameClient.
 */
extern GameClient *g_client;

/**
 * @brief Pointeur global vers l'objet Game.
 */
extern Game *g_game;

/**
 * @brief Mode de jeu global.
 */
extern QString g_gameMode;

/**
 * @brief Rôle du joueur global.
 */
extern QString g_playerRole;

/**
 * @brief Identifiant du joueur global.
 */
extern int g_playerId;

/**
 * @brief Indique si le joueur est l'hôte.
 */
extern bool g_isHost;

#endif