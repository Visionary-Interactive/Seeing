#ifndef SESSIONSTATECONTROLLER_H
#define SESSIONSTATECONTROLLER_H

#include <time.h>
#include "SessionManager.h"
#include "player.h"

#define MAX_PLAYERS 2
#define TICK_RATE_MS 20

extern Player* playerList[MAX_PLAYERS];
extern int clientPlayerCount;
extern Color playerColor, remoteColor;
extern clock_t last_network_tick;

void SessionStateController_Init();
void SessionStateController_Tick(bool isServer);
void CreateNewPlayer();
void InitRemotePlayer();
void NetworkTick(bool isServer);

#endif// SESSIONSTATECONTROLLER_H