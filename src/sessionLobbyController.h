#pragma once

#ifndef SESSIONLOBBYCONTROLLER_H
#define SESSIONLOBBYCONTROLLER_H

#include "sessionManager.h"

extern bool isLobbyFull;

void ConnectToHomeServer();
void SendLobbyQuery();
bool AssignMultiplayerStatus();
bool CheckLobbyStatus();

#endif