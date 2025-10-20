//   ./test server
//   ./test client [server_ip]

#define NBNET_IMPL

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SessionManager.h"

#define SERVER_PORT 12345
#define PROTOCOL_NAME "UDP"
#define CLIENT_CONNECT_TIMEOUT_MS 5000
#define TICK_MS 1000

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <server|client> [host-for-client]\n", argv[0]);
		return 1;
	}

	const char* mode = argv[1];

	// Initialize server
	SessionManager_Init();

	if (strcmp(mode, "server") == 0)
	{
		SessionManager_CreateServer(PROTOCOL_NAME, SERVER_PORT);

		// server loop
		while (true)
		{
			int ev;

			// collect all events and then loop back around
			while ((ev = SessionManager_Server_HandleEvents()) != NBN_NO_EVENT);
			
			// Send packets
			SessionManager_Server_SendPackets();
			sleep_ms(TICK_MS);
		}

		SessionManager_StopServer();
	}
	else if (strcmp(mode, "client") == 0)
	{
		const char* host = (argc >= 3) ? argv[2] : "127.0.0.1"; // set default host to localhost

		SessionManager_CreateClient(PROTOCOL_NAME, host, SERVER_PORT);

		while (true)
		{
			int ev;

			// collect all events and then loop back around
			while ((ev = SessionManager_Client_HandleEvents()) != NBN_NO_EVENT);

			// Send packets
			SessionManager_Client_SendPackets();

			sleep_ms(TICK_MS);
		}
	}

	return 0;
}
