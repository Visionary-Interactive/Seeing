#include "sessionManager.h"

#define NBNET_IMPL

#include "nbnet.h"
#include "net_drivers/udp.h"

clock_t lastNetworkTick = 0;
bool isServer = false;
bool isHost = false;
NBN_ConnectionHandle connectedClientHandle = 0;
struct MovementSnapshot lastMovementSnapshot = { 0 };
struct IncomingPlayer incomingPlayerData = { 0 };
struct PositionSnapshot lastPositionSnapshot = { 0 };
struct LobbyQuery lastLobbyQuery = { 0 };
struct PropInteraction lastPropInteraction = { 0 };
uint32_t peerIP = 0;
uint16_t peerPort = 0;

void (*HostPlayerCallback)() = NULL;
void (*ClientPlayerCallback)() = NULL;
void (*CreatePlayer)() = NULL;
void (*InitalizeRemotePlayer)() = NULL;
void (*PlayerDesyncCorrection)() = NULL;
void (*PropInteractionCallback)() = NULL;
void (*ResetLevelCallback)() = NULL;

void SessionManager_Init()
{
	// Initialize Protocol
	NBN_UDP_Register();
	lastNetworkTick = clock();
	connectedClientHandle = 0;
	lastMovementSnapshot = (struct MovementSnapshot){ 0 };
	incomingPlayerData = (struct IncomingPlayer){ 0 };
	lastPositionSnapshot = (struct PositionSnapshot){ 0 };
	lastLobbyQuery = (struct LobbyQuery){ 0 };
	lastPropInteraction = (struct PropInteraction){ 0 };
	peerIP = 0;
	peerPort = 0;
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
		connectedClientHandle = NBN_GameServer_GetIncomingConnection();
		unsigned int len = NBN_GameServer_ReadIncomingConnectionData(NULL);
		printf("New connection: handle=%u, initial_data_len=%u\n", connectedClientHandle, len);

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
			if (bmsg->length < 1) return 1; // Invalid

			MsgType msg_type = bmsg->bytes[0];
			switch (msg_type)
			{
			case MovementSnapshot:
				if (bmsg->length >= 1 + sizeof(struct MovementSnapshot)) {
					struct MovementSnapshot recv_snap;
					memcpy(&recv_snap, bmsg->bytes + 1, sizeof(struct MovementSnapshot));

					lastMovementSnapshot = recv_snap;
				}
				break;
			case IncomingPlayer:
				if (bmsg->length >= 1 + sizeof(struct IncomingPlayer)) {
					struct IncomingPlayer recv_player;
					memcpy(&recv_player, bmsg->bytes + 1, sizeof(struct IncomingPlayer));

					#if DEBUG_LOGGING >= 1
					printf("Received IncomingPlayer: pos=(%.3f, %.3f, %.3f) scale=(%.3f, %.3f, %.3f) color=(%c, %c, %c) vel=(%.3f, %.3f, %.3f) speed=%.3f yaw=%.3f pitch=%.3f isGrounded=%d\n",
						recv_player.position.x,
						recv_player.position.y,
						recv_player.position.z,
						recv_player.scale.x,
						recv_player.scale.y,
						recv_player.scale.z,
						recv_player.r,
						recv_player.g,
						recv_player.b,
						recv_player.velocity.x,
						recv_player.velocity.y,
						recv_player.velocity.z,
						recv_player.speed,
						recv_player.yaw,
						recv_player.pitch,
						recv_player.isGrounded);
					#endif

					incomingPlayerData = recv_player;
					// Initialize remote player with received data
					InitalizeRemotePlayer();
				}
				break;
			case PositionSnapshot:
				if (bmsg->length >= 1 + sizeof(struct PositionSnapshot)) {
					struct PositionSnapshot recv_pos;
					memcpy(&recv_pos, bmsg->bytes + 1, sizeof(struct PositionSnapshot));
					#if DEBUG_LOGGING >= 1
					printf("Received PositionSnapshot: seq=%d pos=(%d, %d, %d) yaw=%d pitch=%d\n",
						recv_pos.sequence,
						recv_pos.posX,
						recv_pos.posY,
						recv_pos.posZ,
						recv_pos.yaw,
						recv_pos.pitch);
					#endif

					lastPositionSnapshot = recv_pos;
					// Update player position with received data
					PlayerDesyncCorrection();
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

bool SessionManager_Client_IsConnected()
{
	if (NBN_GameClient_IsConnected())
		return true;
	return false;
}

void SessionManager_StopClient()
{
	NBN_GameClient_Stop();
}

bool SessionManager_Client_Disonnect()
{
	bool ret = true;
	if (NBN_GameClient_IsConnected())
	{
		ret = NBN_GameClient_SendMessage(NBN_DISCONNECTION_MESSAGE_TYPE, NBN_CHANNEL_RESERVED_LIBRARY_MESSAGES, NULL) < 0;
		SessionManager_Client_HandleEvents();
		SessionManager_Client_SendPackets();
		lastLobbyQuery.auth = 0; // Reset lobby query state
	}
	return !ret;
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
		//SessionManager_StopClient();
		break;
	}
	case NBN_MESSAGE_RECEIVED:
	{
		NBN_MessageInfo info = NBN_GameClient_GetMessageInfo();
		if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
		{
			NBN_ByteArrayMessage* bmsg = (NBN_ByteArrayMessage*)info.data;
			if (bmsg->length < 1) return 1; // Invalid

			MsgType msg_type = bmsg->bytes[0];
			switch (msg_type)
			{
			case MovementSnapshot:
				if (bmsg->length >= 1 + sizeof(struct MovementSnapshot)) {
					struct MovementSnapshot recv_snap;
					memcpy(&recv_snap, bmsg->bytes + 1, sizeof(struct MovementSnapshot));

					lastMovementSnapshot = recv_snap;
				}
				break;
			case IncomingPlayer:
				if (bmsg->length >= 1 + sizeof(struct IncomingPlayer)) {
					struct IncomingPlayer recv_player;
					memcpy(&recv_player, bmsg->bytes + 1, sizeof(struct IncomingPlayer));
					#if DEBUG_LOGGING >= 1
					printf("Received IncomingPlayer: pos=(%.3f, %.3f, %.3f) scale=(%.3f, %.3f, %.3f) color=(%c, %c, %c) vel=(%.3f, %.3f, %.3f) speed=%.3f yaw=%.3f pitch=%.3f isGrounded=%d\n",
						recv_player.position.x,
						recv_player.position.y,
						recv_player.position.z,
						recv_player.scale.x,
						recv_player.scale.y,
						recv_player.scale.z,
						recv_player.r,
						recv_player.g,
						recv_player.b,
						recv_player.velocity.x,
						recv_player.velocity.y,
						recv_player.velocity.z,
						recv_player.speed,
						recv_player.yaw,
						recv_player.pitch,
						recv_player.isGrounded);
					#endif

					incomingPlayerData = recv_player;
					// Initialize remote player with received data
					InitalizeRemotePlayer();
				}
				break;
			case PositionSnapshot:
				if (bmsg->length >= 1 + sizeof(struct PositionSnapshot)) {
					struct PositionSnapshot recv_pos;
					memcpy(&recv_pos, bmsg->bytes + 1, sizeof(struct PositionSnapshot));
					#if DEBUG_LOGGING >= 1
					printf("Received PositionSnapshot: seq=%d pos=(%d, %d, %d) yaw=%d pitch=%d\n",
						recv_pos.sequence,
						recv_pos.posX,
						recv_pos.posY,
						recv_pos.posZ,
						recv_pos.yaw,
						recv_pos.pitch);
					#endif

					lastPositionSnapshot = recv_pos;
					// Update player position with received data
					PlayerDesyncCorrection();
				}
				break;
			case PropInteraction:
				if (bmsg->length >= 1 + sizeof(struct PropInteraction)) {
					struct PropInteraction recv_propInteraction;
					memcpy(&recv_propInteraction, bmsg->bytes + 1, sizeof(struct PropInteraction));
					#if DEBUG_LOGGING >= 1
						printf("Received PropInteraction from client %u: type=%u propID=%u slot=%u\n",
							info.sender,
							recv_propInteraction.interactType,
							recv_propInteraction.propID,
							recv_propInteraction.selectedSlot);
					#endif

					lastPropInteraction = recv_propInteraction;
					PropInteractionCallback();
				}
				break;
			case LobbyQuery:
				if (bmsg->length >= 1 + sizeof(struct LobbyQuery)) {
					struct LobbyQuery recv_lobbyQuery;
					memcpy(&recv_lobbyQuery, bmsg->bytes + 1, sizeof(struct LobbyQuery));
					#if DEBUG_LOGGING >= 1
						printf("Received LobbyQuery from client %u: auth=%u isHost=%u isFull=%u\n",
							info.sender,
							recv_lobbyQuery.auth,
							recv_lobbyQuery.isHost,
							recv_lobbyQuery.isFull);
						printf("HOST IP: %u.%u.%u.%u:%u\n",
							(recv_lobbyQuery.hostIP >> 24) & 0xFF,
							(recv_lobbyQuery.hostIP >> 16) & 0xFF,
							(recv_lobbyQuery.hostIP >> 8) & 0xFF,
							(recv_lobbyQuery.hostIP) & 0xFF,
							recv_lobbyQuery.hostPort);
					#endif

					peerIP = recv_lobbyQuery.hostIP;
					peerPort = recv_lobbyQuery.hostPort;
					lastLobbyQuery = recv_lobbyQuery;

					// Once we have a response from the traversal server
					if (lastLobbyQuery.auth && lastLobbyQuery.isFull) {
						#if DEBUG_LOGGING >= 1
							printf("Authenticated by server. isHost=%u\n", lastLobbyQuery.isHost);
						#endif
						// Host
						if (lastLobbyQuery.isHost) 
						{
							#if DEBUG_LOGGING >= 1
								printf("Host Player");
							#endif
							isHost = true;
							HostPlayerCallback();
						}
						else // Client
						{
							#if DEBUG_LOGGING >= 1
								printf("Client Player");
							#endif
							isHost = false;
							ClientPlayerCallback();
						}
						#if DEBUG_LOGGING >= 1
							printf("Sending player data across...\n");
						#endif
					}
				}
				break;
			case Retry:
				if (bmsg->length >= 1 + sizeof(struct RetryPacket)) {
					struct RetryPacket recv_RetryPacket;
					memcpy(&recv_RetryPacket, bmsg->bytes + 1, sizeof(struct RetryPacket));
					printf("Received Reset packet\n");
					ResetLevelCallback();
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

// Send player data to the server or client
void SendPlayerData(uint8_t* buffer, unsigned int len, bool isServer)
{
	if (isServer && connectedClientHandle != 0)
		SessionManager_Server_SendReliableByteArray(connectedClientHandle, buffer, len);
	else if (!isServer)
		SessionManager_Client_SendReliableByteArray(buffer, len);
}

// Send player data to the server or client unreliably
void SendUnreliablePlayerData(uint8_t* buffer, unsigned int len, bool isServer)
{
	if (isServer && connectedClientHandle != 0)
		SessionManager_Server_SendUnreliableByteArray(connectedClientHandle, buffer, len);
	else if (!isServer)
		SessionManager_Client_SendUnreliableByteArray(buffer, len);
}
//
//void SendRawToPeer(uint32_t host, uint16_t port, const void* buf, int size)
//{
//	NBN_UDP_SendRawTo(host, port, buf, size);
//}
//
//void ReceiveRawFromPeer(void* buf, int bufsize)
//{
//	NBN_UDP_ReceiveRaw(buf, bufsize);
//}
