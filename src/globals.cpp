#include "../include/globals.h"
#include "../include/game.h"
#include "../client/GameClient/GameClient.h"

Game *g_game = nullptr;
GameClient *g_client = nullptr;
QString g_playerRole = "";
QString g_gameMode = "";
int g_playerId = 0;
bool g_isHost = false;
