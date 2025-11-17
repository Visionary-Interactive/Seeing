#include "SessionStateController.h"

void SessionStateController_Init()
{
	clientPlayerCount = 0;
	last_network_tick = clock();
	CreatePlayer = CreateNewPlayer;
	InitalizeRemotePlayer = InitRemotePlayer;
}

void SessionStateController_Tick(bool isServer)
{
	// Handle networking events at fixed tick rate
	clock_t now = clock();
	int elapsed_ms = (int)(now - last_network_tick);

	if (elapsed_ms >= TICK_RATE_MS)
	{
		last_network_tick = now;
		NetworkTick(isServer);
	}
}

void CreateNewPlayer()
{
	// Create a new remote player and set its properties based on incoming data
	InitPlayer();
	playerList[clientPlayerCount + 1] = GetPlayer();
	playerList[clientPlayerCount + 1]->remotePlayer = true;
	clientPlayerCount++;
}

void InitRemotePlayer()
{
	playerList[clientPlayerCount]->position.x = incomingPlayerData.posX;
	playerList[clientPlayerCount]->position.y = incomingPlayerData.posY;
	playerList[clientPlayerCount]->position.z = incomingPlayerData.posZ;
	playerList[clientPlayerCount]->size.x = incomingPlayerData.scaleX;
	playerList[clientPlayerCount]->size.y = incomingPlayerData.scaleY;
	playerList[clientPlayerCount]->size.z = incomingPlayerData.scaleZ;
	remoteColor.r = incomingPlayerData.r;
	remoteColor.g = incomingPlayerData.g;
	remoteColor.b = incomingPlayerData.b;
	playerList[clientPlayerCount]->velocity.x = incomingPlayerData.velX;
	playerList[clientPlayerCount]->velocity.y = incomingPlayerData.velY;
	playerList[clientPlayerCount]->velocity.z = incomingPlayerData.velZ;
	playerList[clientPlayerCount]->speed = incomingPlayerData.speed;
	playerList[clientPlayerCount]->yaw = incomingPlayerData.yaw;
	playerList[clientPlayerCount]->pitch = incomingPlayerData.pitch;
	playerList[clientPlayerCount]->isGrounded = incomingPlayerData.isGrounded;
}

void NetworkTick(bool isServer)
{
	int ev; // event variable
	if (isServer)
		ev = SessionManager_Server_HandleEvents();
	else
		ev = SessionManager_Client_HandleEvents();

	switch (ev)
	{
	case 2: // NBN_NEW_CONNECTION / NBN_CONNECTED
	{
		// New connection, send the incoming player data
		struct IncomingPlayer incomingPlayer;
		incomingPlayer.posX = playerList[0]->position.x;
		incomingPlayer.posY = playerList[0]->position.y;
		incomingPlayer.posZ = playerList[0]->position.z;
		incomingPlayer.scaleX = playerList[0]->size.x;
		incomingPlayer.scaleY = playerList[0]->size.y;
		incomingPlayer.scaleZ = playerList[0]->size.z;
		incomingPlayer.r = playerColor.r;
		incomingPlayer.g = playerColor.g;
		incomingPlayer.b = playerColor.b;
		incomingPlayer.velX = playerList[0]->velocity.x;
		incomingPlayer.velY = playerList[0]->velocity.y;
		incomingPlayer.velZ = playerList[0]->velocity.z;
		incomingPlayer.speed = playerList[0]->speed;
		incomingPlayer.yaw = playerList[0]->yaw;
		incomingPlayer.pitch = playerList[0]->pitch;
		incomingPlayer.isGrounded = playerList[0]->isGrounded;

		if (isServer)
			SendIncomingPlayer(connectedClientHandle, &incomingPlayer, true);
		else
			SendIncomingPlayer(connectedClientHandle, &incomingPlayer, false);
		break;
	}
	case 3: // NBN_CLIENT_DISCONNECTED / NBN_DISCONNECTED
	{
		printf("A player has disconnected.\n");
		if (clientPlayerCount > 0)
		{
			RL_FREE(playerList[clientPlayerCount]);
			clientPlayerCount--;
		}
		break;
	}
	default:
		break;
	}
	struct Snapshot playerPosition;
	playerPosition.forward = IsKeyDown(KEY_W);
	playerPosition.backward = IsKeyDown(KEY_S);
	playerPosition.left = IsKeyDown(KEY_A);
	playerPosition.right = IsKeyDown(KEY_D);
	playerPosition.y = playerList[0]->position.y;
	playerPosition.pitch = playerList[0]->pitch;
	playerPosition.yaw = playerList[0]->yaw;
	if (clientPlayerCount > 0)
	{
		playerList[1]->ex_W = lastSnapshot.forward;
		playerList[1]->ex_S = lastSnapshot.backward;
		playerList[1]->ex_A = lastSnapshot.left;
		playerList[1]->ex_D = lastSnapshot.right;

		playerList[1]->position.y = lastSnapshot.y;
		playerList[1]->pitch = lastSnapshot.pitch;
		playerList[1]->yaw = lastSnapshot.yaw;
	}
	SessionManager_Tick(playerPosition, isServer);
	if (isServer)
		SessionManager_Server_SendPackets();
	else
		SessionManager_Client_SendPackets();
}