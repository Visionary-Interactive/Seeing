#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MSG_TYPE_SNAPSHOT      1
#define MSG_TYPE_INCOMINGPLAYER 2

typedef uint32_t NBN_ConnectionHandle;

struct Snapshot {
	uint8_t forward;
	uint8_t backward;
	uint8_t left;
	uint8_t right;
	float y;
	float pitch;
	float yaw;
};

struct IncomingPlayer {
	float posX, posY, posZ;
	float scaleX, scaleY, scaleZ;
	unsigned char r, g, b;
	float velX, velY, velZ;
	float speed;
	float yaw;
	float pitch;
	uint8_t isGrounded;
};

extern NBN_ConnectionHandle connectedClientHandle;
extern struct Snapshot lastSnapshot;
extern struct IncomingPlayer incomingPlayerData;

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
void SessionManager_StopClient();
int SessionManager_Client_HandleEvents();
bool SessionManager_Client_SendReliableByteArray(uint8_t* data, unsigned int length);
bool SessionManager_Client_SendUnreliableByteArray(uint8_t* data, unsigned int length);
int SessionManager_Client_SendPackets();

void SendIncomingPlayer(NBN_ConnectionHandle conn, const struct IncomingPlayer* incomingPlayer, bool isServer);
void SessionManager_Tick(struct Snapshot player, bool isServer);

extern void (*CreatePlayer)();
extern void (*InitalizeRemotePlayer)();

#endif