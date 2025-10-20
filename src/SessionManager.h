#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef DEBUG_LOGGING
#define DEBUG_LOGGING 0
#endif

#if DEBUG_LOGGING
// Basic logging to console
#define NBN_LogInfo(...)   printf(__VA_ARGS__); printf("\n")
#define NBN_LogError(...)  printf(__VA_ARGS__); printf("\n")
#define NBN_LogDebug(...)  printf(__VA_ARGS__); printf("\n")
#define NBN_LogTrace(...)  printf(__VA_ARGS__); printf("\n")
#define NBN_LogWarning(...) printf(__VA_ARGS__); printf("\n")
#else
#define NBN_LogInfo(...)    ((void)0)
#define NBN_LogError(...)   ((void)0)
#define NBN_LogDebug(...)   ((void)0)
#define NBN_LogTrace(...)   ((void)0)
#define NBN_LogWarning(...) ((void)0)
#endif

#include "StandardIncludes.h"

void SessionManager_Init();

// Server functions
bool SessionManager_CreateServer(const char* protocol, uint16_t port);
void SessionManager_StopServer();
int SessionManager_Server_HandleEvents();
int SessionManager_Server_SendPackets();

// Client functions
bool SessionManager_CreateClient(const char* protocol, const char* host, uint16_t port);
void SessionManager_StopClient();
int SessionManager_Client_HandleEvents();
bool SessionManager_Client_SendReliableByteArray(const uint8_t* data, unsigned int length);
int SessionManager_Client_SendPackets();

#endif