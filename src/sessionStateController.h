#pragma once

#ifndef SESSIONSTATECONTROLLER_H
#define SESSIONSTATECONTROLLER_H

#include <time.h>
#include "sessionManager.h"
#include "player.h"

#define MAX_PLAYERS 2
#define TICK_RATE_MS 50
#define MAX_BOUNDS 1000

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

int16_t QuantizeFloat(float value, float max_abs);
float DequantizeFloat(int16_t scaled, float max_abs);

#endif// SESSIONSTATECONTROLLER_H