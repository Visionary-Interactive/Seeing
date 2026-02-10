#include "sessionLobbyController.h"

bool isLobbyFull = 0;

void ConnectToHomeServer()
{
	// Connect to home server
	if (!SessionManager_CreateClient("UDP", HOME_SERVER_IP, SERVER_PORT))
	{
		printf("Failed to connect to home server.\n");
	}
	#if DEBUG_LOGGING >= 1
		if (SessionManager_Client_IsConnected())
			printf("Connected to home server at %s:%d\n", HOME_SERVER_IP, SERVER_PORT);
	#endif
}

void SendLobbyQuery()
{
	if (SessionManager_Client_IsConnected())
	{
		#if DEBUG_LOGGING >= 1
			printf("Sending Lobby Query\n");
		#endif
		// Send LobbyQuery to server
		struct LobbyQuery lobbyQuery = { 0 };
		uint8_t buffer[1 + sizeof(struct LobbyQuery)];
		buffer[0] = LobbyQuery; // Set message type
		memcpy(buffer + 1, &lobbyQuery, sizeof(struct LobbyQuery));
		SessionManager_Client_SendReliableByteArray(buffer, sizeof(buffer));
	}
}

bool AssignMultiplayerStatus()
{
	clock_t now = clock();
	int ev;
	if ((int)(now - lastNetworkTick) > 100)
	{
		ev = SessionManager_Client_HandleEvents();
		SessionManager_Client_SendPackets();

		if (ev == 3) // NBN_DISCONNECTED
			ConnectToHomeServer(); // try to reconnect to home server
		else if (ev == 2) // NBN_CONNECTED
			SendLobbyQuery();

		if (SessionManager_Client_IsConnected() && lastLobbyQuery.auth) // Authenticated response by server
		{
			#if DEBUG_LOGGING >= 1
				printf("isHost = %d\n", lastLobbyQuery.isHost);
			#endif

			isHost = lastLobbyQuery.isHost;
			return true;
		}
		lastNetworkTick = now;
	}
	return false;
}

bool CheckLobbyStatus()
{
	if (SessionManager_Client_IsConnected())
	{
		SendLobbyQuery();

		// Check if lobby is full
		SessionManager_Client_HandleEvents();
		SessionManager_Client_SendPackets();
		if (lastLobbyQuery.auth)
		{
			isLobbyFull = lastLobbyQuery.isFull;
			#if DEBUG_LOGGING >= 1
				printf("isLobbyFully = %d\n", isLobbyFull);
			#endif
			return isLobbyFull;
		}
	}
	return false;
}

