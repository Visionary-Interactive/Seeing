#include "SessionManager.h"

void SessionManager_Init()
{
	// Initialize Protocol
	NBN_UDP_Register();
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
		unsigned int len = NBN_GameServer_ReadIncomingConnectionData(NULL); // can pass buffer to read data
		printf("New connection: handle=%u, initial_data_len=%u\n", new_conn, len);

		// Can also use NBN_GameServer_ReadIncomingConnectionData to read initial data sent by client
		// or NBN_GameServer_AcceptIncomingConnectionWithData to accept with data
		if (NBN_GameServer_AcceptIncomingConnection() < 0)
			printf("Warning: failed to accept incoming connection\n");
		else
		{
			const char* server_msg = "Server: Hi client!";
			int rc = NBN_GameServer_SendReliableByteArrayTo(new_conn, (uint8_t*)server_msg, (unsigned int)strlen(server_msg));
			printf("NBN_GameServer_SendUnreliableByteArrayTo returned %d\n", rc);
			//server_wait_for_incoming(2000);

			int sents = NBN_GameServer_SendPackets();
			printf("NBN_GameServer_SendPackets returned %d\n", sents);

			if (rc >= 0)
				printf("Server: queued message to client %u: \"%s\"\n", new_conn, server_msg);
			else
				printf("Server: failed to queue message for client %u\n", new_conn);
		}
		break;
	}
	case NBN_CLIENT_MESSAGE_RECEIVED:
	{
		NBN_MessageInfo info = NBN_GameServer_GetMessageInfo();

		if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
		{
			NBN_ByteArrayMessage* bmsg = (NBN_ByteArrayMessage*)info.data;

			unsigned int l = bmsg->length;
			char* tmp = (char*)malloc(l + 1);
			if (tmp)
			{
				memcpy(tmp, bmsg->bytes, l);
				tmp[l] = '\0'; // add null terminator
				printf("Received from client %u: \"%s\" (len=%u)\n", info.sender, tmp, l);
				free(tmp);
			}
			// echo back
			NBN_GameServer_SendUnreliableByteArrayTo(info.sender, bmsg->bytes, bmsg->length);
			NBN_GameServer_SendPackets();
		}
		else
		{
			printf("Server: received non-byte-array message type=%d\n", info.type);
		}
		break;
	}
	case NBN_CLIENT_DISCONNECTED:
	{
		NBN_ConnectionHandle disc = NBN_GameServer_GetDisconnectedClient();
		printf("Client disconnected: handle=%u\n", disc);
		break;
	}
	case NBN_NO_EVENT:
	default:
		break;
	}

	return ev;
}

bool SessionManager_Server_SendReliableByteArray(NBN_ConnectionHandle conn, const uint8_t* data, unsigned int length)
{
	if (NBN_GameServer_SendReliableByteArrayTo(conn, (uint8_t*)data, length) < 0)
	{
		printf("Failed to send Reliable Byte Array to client %u\n", conn);
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

		const char* client_msg = "Client: Hi server!";
		int rc = NBN_GameClient_SendReliableByteArray((uint8_t*)client_msg, (unsigned int)strlen(client_msg));
		printf("NBN_GameClient_SendUnreliableByteArray returned %d\n", rc);

		int sents = NBN_GameClient_SendPackets();
		printf("NBN_GameClient_SendPackets returned %d\n", sents);

		if (rc >= 0)
			printf("Client: queued initial message: \"%s\"\n", client_msg);
		else
			printf("Client: failed to queue initial message\n");
		break;
	}
	case NBN_DISCONNECTED:
	{
		printf("Disconnected while connecting\n");
		break;
	}
	case NBN_MESSAGE_RECEIVED:
	{
		NBN_MessageInfo info = NBN_GameClient_GetMessageInfo();
		if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
		{
			NBN_ByteArrayMessage* b = (NBN_ByteArrayMessage*)info.data;
			char* tmp = malloc(b->length + 1);
			if (tmp)
			{
				memcpy(tmp, b->bytes, b->length);
				tmp[b->length] = '\0';
				printf("Got message \"%s\"\n", tmp);
				free(tmp);
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

bool SessionManager_Client_SendReliableByteArray(const uint8_t* data, unsigned int length)
{
	if (NBN_GameClient_SendReliableByteArray((uint8_t*)data, length) < 0)
	{
		printf("Failed to send Reliable Byte Array\n");
		return false;
	}
	return true;
}

// Send queued packets
int SessionManager_Client_SendPackets()
{
	return NBN_GameClient_SendPackets();;
}
