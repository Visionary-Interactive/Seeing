#pragma once

#ifndef SESSIONSTATECONTROLLER_H
#define SESSIONSTATECONTROLLER_H

#include <time.h>
#include "sessionManager.h"
#include "player.h"

#define MAX_PLAYERS 2
#define TICK_RATE_MS 50

extern Player* playerList[MAX_PLAYERS];
extern uint8_t clientPlayerCount;
extern Color playerColor, remoteColor;
extern clock_t lastNetworkTick;
extern uint16_t localSequence;
extern uint16_t lastPositionSequence;

void SessionStateController_Init();
void SessionStateController_Tick(bool isServer);
void CreateNewPlayer();
void InitRemotePlayer();
void UpdatePlayerPosition();
void NetworkTick(bool isServer);
void NetworkCorrectionTick(bool isServer);

#endif// SESSIONSTATECONTROLLER_H