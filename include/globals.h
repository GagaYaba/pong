#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QHostAddress>

class GameClient;

extern GameClient *g_client;
extern QString g_gameMode;
extern QString g_playerRole;
extern int g_playerId;
extern bool g_isHost;

#endif
