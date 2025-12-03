#pragma once

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t NBN_ConnectionHandle;

typedef enum {
	MovementSnapshot,
	PositionSnapshot,
	IncomingPlayer
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

extern NBN_ConnectionHandle connectedClientHandle;
extern struct MovementSnapshot lastMovementSnapshot;
extern struct IncomingPlayer incomingPlayerData;
extern struct PositionSnapshot lastPositionSnapshot;
extern Rectangle test;
extern Rectangle test;

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

void SendPlayerData(uint8_t* buffer, unsigned int len, bool isServer);
void SendUnreliablePlayerData(uint8_t* buffer, unsigned int len, bool isServer);

extern void (*CreatePlayer)();
extern void (*InitalizeRemotePlayer)();
extern void (*PlayerDesyncCorrection)();

#endif SESSIONMANAGER_H