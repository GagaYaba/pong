#include "../include/globals.h"
#include "../include/game.h"
#include "../client/GameClient/GameClient.h"

/**
 * @brief Pointeur global vers l'objet Game.
 */
Game *g_game = nullptr;

/**
 * @brief Pointeur global vers l'objet GameClient.
 */
GameClient *g_client = nullptr;

/**
 * @brief Rôle du joueur dans le jeu.
 */
QString g_playerRole = "";

/**
 * @brief Mode de jeu actuel.
 */
QString g_gameMode = "";

/**
 * @brief Identifiant du joueur.
 */
int g_playerId = 0;

/**
 * @brief Indique si le joueur est l'hôte de la partie.
 */
bool g_isHost = false;