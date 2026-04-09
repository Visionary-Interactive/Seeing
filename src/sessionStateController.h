#pragma once

#ifndef SESSIONSTATECONTROLLER_H
#define SESSIONSTATECONTROLLER_H

#include "sessionManager.h"
#include "player.h"
#include "prop.h"

#define TICK_RATE_MS 25
#define MAX_BOUNDS 1000

extern bool multiplayerSession;
extern uint8_t clientPlayerCount;
extern Color playerColor, remoteColor;
extern uint16_t localSequence;
extern uint16_t lastPositionSequence;

void SessionStateController_Init();
void SessionStateController_Tick(bool isServer);
void CreateNewPlayer();
void InitRemotePlayer();
void SendPlayerDataToRemote();
void UpdatePlayerPosition();
void PropInteractionRPC();
void SendPropInteraction(InteractionType interaction, int selectedSlot, int propID);
void SendRetryLevel();
void SendFinishLevel();
void NetworkTick(bool isServer);
void NetworkCorrectionTick(bool isServer);
void OnPlayerDisconnect();


int16_t QuantizeFloat(float value, float max_abs);
float DequantizeFloat(int16_t scaled, float max_abs);

#endif