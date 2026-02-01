#pragma once

#ifndef SESSIONLOBBYCONTROLLER_H
#define SESSIONLOBBYCONTROLLER_H

#include "sessionManager.h"

extern bool isHost;
extern bool isLobbyFull;

void SendLobbyQuery();
void ConnectToHomeServer();
bool AssignMultiplayerStatus();
bool CheckLobbyStatus();

#endif