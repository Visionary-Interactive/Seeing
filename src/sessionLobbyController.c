#include "sessionLobbyController.h"

bool isHost = 0;
bool isLobbyFull = 0;

void ConnectToHomeServer()
{
	// Connect to home server
	while (!SessionManager_CreateClient("UDP", HOME_SERVER_IP, SERVER_PORT))
	{
		printf("Failed to connect to home server. Retrying...\n");
	}
	#if DEBUG_LOGGING >= 1
		printf("Connected to home server at %s:%d\n", HOME_SERVER_IP, SERVER_PORT);
	#endif
}

void SendLobbyQuery()
{
	if (SessionManager_Client_IsConnected())
	{
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
	if (SessionManager_Client_IsConnected())
	{
		SendLobbyQuery();

		#if DEBUG_LOGGING >= 1
			printf("Waiting for response from server...");
		#endif
		// Wait for response from server
		while (!lastLobbyQuery.auth)
		{
			SessionManager_Client_HandleEvents();
		}
		#if DEBUG_LOGGING >= 1
			printf("isHost = %d", lastLobbyQuery.isHost);
		#endif

		isHost = lastLobbyQuery.isHost;
		return true;
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
		if (lastLobbyQuery.auth)
		{
			isLobbyFull = lastLobbyQuery.isFull;
			#if DEBUG_LOGGING >= 1
				printf("isLobbyFully = %d", isLobbyFull);
			#endif
			return isLobbyFull;
		}
	}
	return false;
}

