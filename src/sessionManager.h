#pragma once

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define SERVER_PORT 1337
#define HOME_SERVER_IP "76.64.212.254"

#ifndef DEBUG_LOGGING
#define DEBUG_LOGGING 2
#endif

#if DEBUG_LOGGING == 1
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

typedef uint32_t NBN_ConnectionHandle;

typedef enum {
	MovementSnapshot,
	PositionSnapshot,
	IncomingPlayer,
	PropInteraction,
	LobbyQuery
} MsgType;

struct SessionVec3 {
	float x;
	float y;
	float z;
};

struct MovementSnapshot { // Sent every tick
	uint16_t sequence;
	uint8_t forward : 1;
	uint8_t backward : 1;
	uint8_t left : 1;
	uint8_t right : 1;
	uint8_t sprint : 1;
	uint8_t interact : 1;
	uint8_t place : 1;
	uint8_t selectedSlot : 4;
	int16_t y;
	int16_t pitch;
	int16_t yaw;
};

struct PositionSnapshot { // Sent occasionally to correct position
	uint16_t sequence;
	int16_t posX, posY, posZ;
	int16_t yaw;
	int16_t pitch;
};

struct IncomingPlayer { // Sent once upon connection
	struct SessionVec3 position;
	struct SessionVec3 scale;
	unsigned char r, g, b;
	struct SessionVec3 velocity;
	float speed;
	float yaw;
	float pitch;
	uint8_t isGrounded;
};

struct PropInteraction { // Sent when a prop/object is interacted with by the local player
	uint8_t interactType;
	uint8_t propID;
	uint8_t selectedSlot;
};

struct LobbyQuery { // Sent upon joining lobby
	uint8_t auth : 1;	// Authenticated by server
	uint8_t isHost : 1;
	uint8_t isFull : 1;
	uint32_t hostIP;
	uint16_t hostPort;
};

extern clock_t lastNetworkTick;
extern bool isServer;
extern bool isHost;
extern NBN_ConnectionHandle connectedClientHandle;
extern struct MovementSnapshot lastMovementSnapshot;
extern struct IncomingPlayer incomingPlayerData;
extern struct PositionSnapshot lastPositionSnapshot;
extern struct LobbyQuery lastLobbyQuery;
extern struct PropInteraction lastPropInteraction;
extern uint32_t peerIP;
extern uint16_t peerPort;


void SessionManager_Init();

// Server functions
bool SessionManager_CreateServer(const char* protocol, uint16_t port);
void SessionManager_StopServer();
int SessionManager_Server_HandleEvents();
bool SessionManager_Server_SendReliableByteArray(NBN_ConnectionHandle conn, uint8_t* data, unsigned int length);
bool SessionManager_Server_SendUnreliableByteArray(NBN_ConnectionHandle conn, uint8_t* data, unsigned int length);
int SessionManager_Server_SendPackets();

// Client functions
bool SessionManager_CreateClient(const char* protocol, const char* host, uint16_t port);
bool SessionManager_Client_IsConnected();
void SessionManager_StopClient();
int SessionManager_Client_HandleEvents();
bool SessionManager_Client_SendReliableByteArray(uint8_t* data, unsigned int length);
bool SessionManager_Client_SendUnreliableByteArray(uint8_t* data, unsigned int length);
int SessionManager_Client_SendPackets();

void SendPlayerData(uint8_t* buffer, unsigned int len, bool isServer);
void SendUnreliablePlayerData(uint8_t* buffer, unsigned int len, bool isServer);
//void SendRawToPeer(uint32_t host, uint16_t port, const void* buf, int size);
//void ReceiveRawFromPeer(void *buf, int bufsize);

extern void (*HostPlayerCallback)();
extern void (*ClientPlayerCallback)();
extern void (*CreatePlayer)();
extern void (*InitalizeRemotePlayer)();
extern void (*PlayerDesyncCorrection)();
extern void (*PropInteractionCallback)();

#endif // SESSIONMANAGER_H