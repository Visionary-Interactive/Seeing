//   ./test server
//   ./test client [server_ip]

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

#define SERVER_PORT 12345
#define PROTOCOL_NAME "UDP"
#define CLIENT_CONNECT_TIMEOUT_MS 5000
#define TICK_MS 1000

static void server_wait_for_incoming(int max_ms)
{
	int waited = 0;
	while (waited < max_ms)
	{
		int ev;
		while ((ev = NBN_GameServer_Poll()) != NBN_NO_EVENT)
		{
			if (ev == NBN_CLIENT_MESSAGE_RECEIVED)
			{
				NBN_MessageInfo info = NBN_GameServer_GetMessageInfo();
				if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
				{
					NBN_ByteArrayMessage* b = (NBN_ByteArrayMessage*)info.data;
					char* tmp = (char*)malloc(b->length + 1);
					if (tmp)
					{
						memcpy(tmp, b->bytes, b->length);
						tmp[b->length] = '\0';
						printf("<< SERVER RECEIVED from client %u: \"%s\"\n", info.sender, tmp);
						free(tmp);
					}
				}
				else
				{
					printf("<< SERVER RECEIVED message type=%u\n", info.type);
				}
			}
			else if (ev == NBN_CLIENT_DISCONNECTED)
			{
				printf("<< SERVER EVENT: CLIENT_DISCONNECTED\n");
			}
			else if (ev == NBN_NEW_CONNECTION)
			{
				printf("<< SERVER EVENT: NEW_CONNECTION\n");
				// consume as before
				NBN_ConnectionHandle h = NBN_GameServer_GetIncomingConnection();
				NBN_GameServer_ReadIncomingConnectionData(NULL);
				NBN_GameServer_AcceptIncomingConnection();
			}
		}
		// Ensure we flush any outgoing packets (not necessary for receive but harmless)
		NBN_GameServer_SendPackets();
		sleep_ms(20);
		waited += 20;
	}
}

static void client_wait_for_incoming(int max_ms)
{
	int waited = 0;
	while (waited < max_ms)
	{
		int ev;
		while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT)
		{
			if (ev == NBN_MESSAGE_RECEIVED)
			{
				NBN_MessageInfo info = NBN_GameClient_GetMessageInfo();
				if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
				{
					NBN_ByteArrayMessage* b = (NBN_ByteArrayMessage*)info.data;
					char* tmp = (char*)malloc(b->length + 1);
					if (tmp)
					{
						memcpy(tmp, b->bytes, b->length);
						tmp[b->length] = '\0';
						printf("<< CLIENT RECEIVED from server: \"%s\"\n", tmp);
						free(tmp);
					}
				}
				else
				{
					printf("<< CLIENT RECEIVED message type=%u\n", info.type);
				}
			}
			else if (ev == NBN_DISCONNECTED)
			{
				printf("<< CLIENT EVENT: DISCONNECTED\n");
			}
		}
		NBN_GameClient_SendPackets();
		sleep_ms(20);
		waited += 20;
	}
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <server|client> [host-for-client]\n", argv[0]);
		return 1;
	}

	const char* mode = argv[1];

	// Initialize server
	NBN_UDP_Register();

	if (strcmp(mode, "server") == 0)
	{
		if (NBN_GameServer_Start(PROTOCOL_NAME, SERVER_PORT) < 0)
		{
			printf("Failed to start server\n");
			return 1;
		}

		printf("Server started on port %d\n", SERVER_PORT);

		// server loop
		while (true)
		{
			int ev;

			// collect all events and then loop back around
			while ((ev = NBN_GameServer_Poll()) != NBN_NO_EVENT)
			{

				// handle events
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
						int rc = NBN_GameServer_SendUnreliableByteArrayTo(new_conn, (uint8_t*)server_msg, (unsigned int)strlen(server_msg));
						printf("NBN_GameServer_SendUnreliableByteArrayTo returned %d\n", rc);
						server_wait_for_incoming(2000);

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
			}

			// Send packets
			NBN_GameServer_SendPackets();
			sleep_ms(TICK_MS);
		}

		NBN_GameServer_Stop();
	}
	else if (strcmp(mode, "client") == 0)
	{
		const char* host = (argc >= 3) ? argv[2] : "127.0.0.1"; // set default host to localhost

		if (NBN_GameClient_Start(PROTOCOL_NAME, host, SERVER_PORT) < 0)
		{
			printf("Failed to start client\n");
			return 1;
		}

		printf("Client started. Connecting to %s:%d ...\n", host, SERVER_PORT);

		// Wait up to CLIENT_CONNECT_TIMEOUT_MS for connection
		int elapsed = 0;
		bool connected = false;
		while (elapsed < CLIENT_CONNECT_TIMEOUT_MS)
		{
			int ev;

			// collect all events and then loop back around
			while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT)
			{
				if (ev == NBN_CONNECTED)
				{
					printf("Connected to server\n");
					connected = true;

					const char* client_msg = "Client: Hi server!";
					int rc = NBN_GameClient_SendReliableByteArray((uint8_t*)client_msg, (unsigned int)strlen(client_msg));
					printf("NBN_GameClient_SendUnreliableByteArray returned %d\n", rc);
					client_wait_for_incoming(2000);

					int sents = NBN_GameClient_SendPackets();
					printf("NBN_GameClient_SendPackets returned %d\n", sents);

					if (rc >= 0)
						printf("Client: queued initial message: \"%s\"\n", client_msg);
					else
						printf("Client: failed to queue initial message\n");
				}
				else if (ev == NBN_DISCONNECTED)
				{
					printf("Disconnected while connecting\n");
				}
				else if (ev == NBN_MESSAGE_RECEIVED)
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
				}
			}
			if (connected) break;

			// Send packets
			NBN_GameClient_SendPackets();

			sleep_ms(TICK_MS);
			elapsed += TICK_MS;
		}

		if (!connected)
		{
			printf("Connection timeout\n");
			NBN_GameClient_Stop();
			return 1;
		}

		// Send a test message
		const char* payload = "Hello from client";
		if (NBN_GameClient_SendReliableByteArray((uint8_t*)payload, (unsigned int)strlen(payload)) < 0)
			printf("Client: failed to queue message\n");

		// Send packets
		NBN_GameClient_SendPackets();

		// Main client loop - process incoming messages and exit on disconnect
		while (true)
		{
			int ev;
			while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT)
			{
				if (ev == NBN_MESSAGE_RECEIVED)
				{
					NBN_MessageInfo info = NBN_GameClient_GetMessageInfo();
					if (info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE)
					{
						NBN_ByteArrayMessage* bmsg = (NBN_ByteArrayMessage*)info.data;
						unsigned int l = bmsg->length;
						char* tmp = (char*)malloc(l + 1);
						if (tmp)
						{
							memcpy(tmp, bmsg->bytes, l);
							tmp[l] = '\0';
							printf("<< RECEIVED (type %u, len %u) : \"%s\"\n", info.type, l, tmp);
							free(tmp);
						}
					}
				}
				else if (ev == NBN_DISCONNECTED)
				{
					int code = NBN_GameClient_GetServerCloseCode();
					printf("Client: disconnected (code=%d)\n", code);
					goto client_stop; // bad
				}
				else if (ev == NBN_CONNECTED)
				{
				}
			}

			NBN_GameClient_SendPackets();

			sleep_ms(TICK_MS);
		}

	client_stop:
		NBN_GameClient_Stop();
	}
	else
	{
		return 1;
	}

	return 0;
}
