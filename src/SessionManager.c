#include "SessionManager.h"

#define NBNET_IMPL

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

#include "nbnet.h"
#include "net_drivers/udp.h"

NBN_ConnectionHandle connectedClientHandle = 0;
struct Snapshot lastSnapshot = { 0 };
struct IncomingPlayer incomingPlayerData = { 0 };

void (*CreatePlayer)() = NULL;
void (*InitalizeRemotePlayer)() = NULL;

void SessionManager_Init()
{
	// Initialize Protocol
	NBN_UDP_Register();
	connectedClientHandle = 0;
	lastSnapshot = (struct Snapshot){ 0 };
}

// Create a server session
bool SessionManager_CreateServer(const char* protocol, uint16_t port)
{
	if (NBN_GameServer_Start(protocol, port) < 0)
	{
		printf("Failed to start server\n");
		return false;
	}
	printf("Server started on port %d\n", port);
	return true;
}

void SessionManager_StopServer()
{
	NBN_GameServer_Stop();
}

// Poll and handle server events
int SessionManager_Server_HandleEvents()
{
	int ev = NBN_GameServer_Poll();

	switch (ev)
	{
	case NBN_NEW_CONNECTION:
	{
		NBN_ConnectionHandle new_conn = NBN_GameServer_GetIncomingConnection();
		connectedClientHandle = new_conn;
		unsigned int len = NBN_GameServer_ReadIncomingConnectionData(NULL); // can pass buffer to read data
		printf("New connection: handle=%u, initial_data_len=%u\n", new_conn, len);

		if (NBN_GameServer_AcceptIncomingConnection() < 0)
			printf("Warning: failed to accept incoming connection\n");

		// Create player
		CreatePlayer();

		break;
	}
	case NBN_CLIENT_MESSAGE_RECEIVED:
	{
		NBN_MessageInfo info = NBN_GameServer_GetMessageInfo();

		if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
		{
			NBN_ByteArrayMessage* bmsg = (NBN_ByteArrayMessage*)info.data;
			if (bmsg->length < 1) return -1; // Invalid

			uint8_t msg_type = bmsg->bytes[0];
			switch (msg_type)
			{
			case MSG_TYPE_SNAPSHOT:
				if (bmsg->length >= 1 + sizeof(struct Snapshot)) {
					struct Snapshot recv_snap;
					memcpy(&recv_snap, bmsg->bytes + 1, sizeof(struct Snapshot));

					lastSnapshot = recv_snap;
				}
				break;
			case MSG_TYPE_INCOMINGPLAYER:
				if (bmsg->length >= 1 + sizeof(struct IncomingPlayer)) {
					struct IncomingPlayer recv_player;
					memcpy(&recv_player, bmsg->bytes + 1, sizeof(struct IncomingPlayer));
					printf("Received IncomingPlayer: pos=(%.3f, %.3f, %.3f) scale=(%.3f, %.3f, %.3f) color=(%c, %c, %c) vel=(%.3f, %.3f, %.3f) speed=%.3f yaw=%.3f pitch=%.3f isGrounded=%d\n",
						recv_player.posX,
						recv_player.posY,
						recv_player.posZ,
						recv_player.scaleX,
						recv_player.scaleY,
						recv_player.scaleZ,
						recv_player.r,
						recv_player.g,
						recv_player.b,
						recv_player.velX,
						recv_player.velY,
						recv_player.velZ,
						recv_player.speed,
						recv_player.yaw,
						recv_player.pitch,
						recv_player.isGrounded);

					incomingPlayerData = recv_player;
					// Initialize remote player with received data
					InitalizeRemotePlayer();
				}
				break;
			default:
				// Unknown type
				break;
			}
		}
		break;
	}
	case NBN_CLIENT_DISCONNECTED:
	{
		NBN_ConnectionHandle disc = NBN_GameServer_GetDisconnectedClient();
		printf("Client disconnected: handle=%u\n", disc);
		connectedClientHandle = 0;
		break;
	}
	case NBN_NO_EVENT:
	default:
		break;
	}

	return ev;
}

bool SessionManager_Server_SendReliableByteArray(NBN_ConnectionHandle conn, uint8_t* data, unsigned int length)
{
	if (NBN_GameServer_SendReliableByteArrayTo(conn, data, length) < 0)
	{
		printf("Failed to send Reliable Byte Array to client %u\n", conn);
		return false;
	}
	return true;
}

bool SessionManager_Server_SendUnreliableByteArray(NBN_ConnectionHandle conn, uint8_t* data, unsigned int length)
{
	if (NBN_GameServer_SendUnreliableByteArrayTo(conn, data, length) < 0)
	{
		printf("Failed to send Unreliable Byte Array to client %u\n", conn);
		return false;
	}
	return true;
}

// Send queued packets
int SessionManager_Server_SendPackets()
{
	return NBN_GameServer_SendPackets();
}

// Create a client session connected to given host and port
bool SessionManager_CreateClient(const char* protocol, const char* host, uint16_t port)
{
	if (NBN_GameClient_Start(protocol, host, port) < 0)
	{
		printf("Failed to start client\n");
		return false;
	}
	printf("Client started. Connecting to %s:%d ...\n", host, port);
	return true;
}

void SessionManager_StopClient()
{
	NBN_GameClient_Stop();
}

// Poll and handle client events
int SessionManager_Client_HandleEvents()
{
	int ev = NBN_GameClient_Poll();

	switch (ev)
	{
	case NBN_CONNECTED:
	{
		printf("Connected to server\n");

		// Create player
		CreatePlayer();

		break;
	}
	case NBN_DISCONNECTED:
	{
		printf("Disconnected from server\n");
		SessionManager_StopClient();
		break;
	}
	case NBN_MESSAGE_RECEIVED:
	{
		NBN_MessageInfo info = NBN_GameClient_GetMessageInfo();
		if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
		{
			NBN_ByteArrayMessage* bmsg = (NBN_ByteArrayMessage*)info.data;
			if (bmsg->length < 1) return -1; // Invalid

			uint8_t msg_type = bmsg->bytes[0];
			switch (msg_type)
			{
			case MSG_TYPE_SNAPSHOT:
				if (bmsg->length >= 1 + sizeof(struct Snapshot)) {
					struct Snapshot recv_snap;
					memcpy(&recv_snap, bmsg->bytes + 1, sizeof(struct Snapshot));
					//printf("Received Snapshot: forward=%d \tbackward=%d \tleft=%d \tright=%d \ty=%.3f \tpitch=%.3f \tyaw=%.3f\n",
					//	recv_snap.forward,
					//	recv_snap.backward,
					//	recv_snap.left,
					//	recv_snap.right,
					//	recv_snap.y,
					//	recv_snap.pitch,
					//	recv_snap.yaw);

					lastSnapshot = recv_snap;
				}
				break;
			case MSG_TYPE_INCOMINGPLAYER:
				if (bmsg->length >= 1 + sizeof(struct IncomingPlayer)) {
					struct IncomingPlayer recv_player;
					memcpy(&recv_player, bmsg->bytes + 1, sizeof(struct IncomingPlayer));
					printf("Received IncomingPlayer: pos=(%.3f, %.3f, %.3f) scale=(%.3f, %.3f, %.3f) color=(%c, %c, %c) vel=(%.3f, %.3f, %.3f) speed=%.3f yaw=%.3f pitch=%.3f isGrounded=%d\n",
						recv_player.posX,
						recv_player.posY,
						recv_player.posZ,
						recv_player.scaleX,
						recv_player.scaleY,
						recv_player.scaleZ,
						recv_player.r,
						recv_player.g,
						recv_player.b,
						recv_player.velX,
						recv_player.velY,
						recv_player.velZ,
						recv_player.speed,
						recv_player.yaw,
						recv_player.pitch,
						recv_player.isGrounded);

					incomingPlayerData = recv_player;
					// Initialize remote player with received data
					InitalizeRemotePlayer();
				}
				break;
			default:
				// Unknown type
				break;
			}
		}
		break;
	}
	case NBN_NO_EVENT:
	default:
		break;
	}

	return ev;
}

bool SessionManager_Client_SendReliableByteArray(uint8_t* data, unsigned int length)
{
	if (NBN_GameClient_SendReliableByteArray((uint8_t*)data, length) < 0)
	{
		printf("Failed to send Reliable Byte Array\n");
		return false;
	}
	return true;
}

bool SessionManager_Client_SendUnreliableByteArray(uint8_t* data, unsigned int length)
{
	if (NBN_GameClient_SendUnreliableByteArray(data, length) < 0)
	{
		printf("Failed to send Unreliable Byte Array\n");
		return false;
	}
	return true;
}

// Send queued packets
int SessionManager_Client_SendPackets()
{
	return NBN_GameClient_SendPackets();
}

// Send initial player data
void SendIncomingPlayer(NBN_ConnectionHandle conn, const struct IncomingPlayer* incomingPlayer, bool isServer)
{
	uint8_t buffer[1 + sizeof(struct IncomingPlayer)];
	buffer[0] = MSG_TYPE_INCOMINGPLAYER;
	memcpy(buffer + 1, incomingPlayer, sizeof(struct IncomingPlayer));
	if (isServer)
		SessionManager_Server_SendReliableByteArray(connectedClientHandle, buffer, sizeof(buffer));
	else
		SessionManager_Client_SendReliableByteArray(buffer, sizeof(buffer));
}

// Server/Client tick function
void SessionManager_Tick(struct Snapshot player, bool isServer)
{
	uint8_t buffer[1 + sizeof(player)];
	buffer[0] = MSG_TYPE_SNAPSHOT;
	memcpy(buffer + 1, &player, sizeof(player));
	if (isServer && connectedClientHandle != 0)
		SessionManager_Server_SendReliableByteArray(connectedClientHandle, buffer, (unsigned int)sizeof(buffer));
	else if (!isServer)
		SessionManager_Client_SendReliableByteArray(buffer, (unsigned int)sizeof(buffer));
}
