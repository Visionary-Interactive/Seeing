#include "sessionStateController.h"

Player* playerList[MAX_PLAYERS];
uint8_t clientPlayerCount = 0;
Color playerColor;
Color remoteColor;
clock_t lastNetworkTick = 0;
uint16_t localSequence = 1;
uint16_t lastPositionSequence = 0;

void SessionStateController_Init()
{
	clientPlayerCount = 0;
	lastNetworkTick = clock();
	CreatePlayer = CreateNewPlayer; // To be called when a new player connects
	InitalizeRemotePlayer = InitRemotePlayer; // To be called to initialize remote player data
	PlayerDesyncCorrection = UpdatePlayerPosition; // To be called to update player position
}

void SessionStateController_Tick(bool isServer)
{
	// Handle networking events at fixed tick rate
	clock_t now = clock();
	int elapsed_ms = (int)(now - lastNetworkTick);

	if ((localSequence % 500) == 0) // Every 500 ticks
	{
		NetworkCorrectionTick(isServer);
		localSequence++;
	}
	else if (elapsed_ms >= TICK_RATE_MS)
	{
		lastNetworkTick = now;
		NetworkTick(isServer);
	}
}

// Called when a new player connects
void CreateNewPlayer()
{
	// Create a new remote player and set its properties based on incoming data
	InitPlayer();
	playerList[clientPlayerCount + 1] = GetPlayer();
	playerList[clientPlayerCount + 1]->remotePlayer = true;
	clientPlayerCount++;
}

// Called to initialize remote player data
void InitRemotePlayer()
{
	playerList[clientPlayerCount]->position = (Vector3){
		incomingPlayerData.position.x,
		incomingPlayerData.position.y,
		incomingPlayerData.position.z
	};
	playerList[clientPlayerCount]->size = (Vector3){
		incomingPlayerData.scale.x,
		incomingPlayerData.scale.y,
		incomingPlayerData.scale.z
	};
	remoteColor.r = incomingPlayerData.r;
	remoteColor.g = incomingPlayerData.g;
	remoteColor.b = incomingPlayerData.b;
	playerList[clientPlayerCount]->velocity = (Vector3){
		incomingPlayerData.velocity.x,
		incomingPlayerData.velocity.y,
		incomingPlayerData.velocity.z
	};
	playerList[clientPlayerCount]->speed = incomingPlayerData.speed;
	playerList[clientPlayerCount]->yaw = incomingPlayerData.yaw;
	playerList[clientPlayerCount]->pitch = incomingPlayerData.pitch;
	playerList[clientPlayerCount]->isGrounded = incomingPlayerData.isGrounded;
}

// Called to update remote player position based on received snapshot
void UpdatePlayerPosition()
{
	printf("last position seq: %d, last remote position seq: %d\n", 
		lastPositionSequence, lastPositionSnapshot.sequence);
	if (clientPlayerCount > 0 
		&& lastPositionSnapshot.sequence > lastPositionSequence) // Only apply if new data
	{
		lastPositionSequence = lastPositionSnapshot.sequence;
		playerList[1]->position = (Vector3){
			DequantizeFloat(lastPositionSnapshot.posX, MAX_BOUNDS),
			DequantizeFloat(lastPositionSnapshot.posY, MAX_BOUNDS),
			DequantizeFloat(lastPositionSnapshot.posZ, MAX_BOUNDS)
		};
		playerList[1]->yaw = DequantizeFloat(lastPositionSnapshot.yaw, MAX_BOUNDS);
		playerList[1]->pitch = DequantizeFloat(lastPositionSnapshot.pitch, MAX_BOUNDS);
		printf("lastPositionSnapshot: %d, %d, %d\tyaw: %d, pitch: %d\n",
			lastPositionSnapshot.posX,
			lastPositionSnapshot.posY,
			lastPositionSnapshot.posZ,
			lastPositionSnapshot.yaw,
			lastPositionSnapshot.pitch);
		printf("Updated remote player position to: %.3f, %.3f, %.3f\tyaw: %.3f, pitch: %.3f\n",
			playerList[1]->position.x,
			playerList[1]->position.y,
			playerList[1]->position.z,
			playerList[1]->yaw,
			playerList[1]->pitch);
	}
}

// Network tick function - runs often
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
		incomingPlayer.position = (struct SessionVec3){
			playerList[0]->position.x,
			playerList[0]->position.y,
			playerList[0]->position.z
		};
		incomingPlayer.scale = (struct SessionVec3){
			playerList[0]->size.x,
			playerList[0]->size.y,
			playerList[0]->size.z
		};
		incomingPlayer.r = playerColor.r;
		incomingPlayer.g = playerColor.g;
		incomingPlayer.b = playerColor.b;
		incomingPlayer.velocity = (struct SessionVec3){
			playerList[0]->velocity.x,
			playerList[0]->velocity.y,
			playerList[0]->velocity.z
		};
		incomingPlayer.speed = playerList[0]->speed;
		incomingPlayer.yaw = playerList[0]->yaw;
		incomingPlayer.pitch = playerList[0]->pitch;
		incomingPlayer.isGrounded = playerList[0]->isGrounded;

		// Send IncomingPlayer packet
		uint8_t buffer[1 + sizeof(struct IncomingPlayer)];
		buffer[0] = IncomingPlayer; // Set message type
		memcpy(buffer + 1, &incomingPlayer, sizeof(struct IncomingPlayer));
		SendPlayerData(buffer, sizeof(buffer), isServer);

		break;
	}
	case 3: // NBN_CLIENT_DISCONNECTED / NBN_DISCONNECTED
	{
		printf("A player has disconnected.\n");
		if (clientPlayerCount > 0)
		{
			RL_FREE(playerList[clientPlayerCount]);
			playerList[clientPlayerCount] = NULL;
			clientPlayerCount--;
		}
		break;
	}
	default:
		break;
	}

	struct MovementSnapshot movementSnapshot;
	movementSnapshot.sequence = localSequence;
	movementSnapshot.forward = IsKeyDown(KEY_W);
	movementSnapshot.backward = IsKeyDown(KEY_S);
	movementSnapshot.left = IsKeyDown(KEY_A);
	movementSnapshot.right = IsKeyDown(KEY_D);
	movementSnapshot.interact = IsKeyDown(KEY_E);
	movementSnapshot.place = IsKeyDown(KEY_R);
	movementSnapshot.sprint = IsKeyDown(KEY_LEFT_SHIFT);
	movementSnapshot.y = QuantizeFloat(playerList[0]->position.y, MAX_BOUNDS);
	movementSnapshot.pitch = QuantizeFloat(playerList[0]->pitch, MAX_BOUNDS);
	movementSnapshot.yaw = QuantizeFloat(playerList[0]->yaw, MAX_BOUNDS);

	// Waits for a new position snapshot every 500 ticks before updating remote player input
	if (clientPlayerCount > 0 && playerList[1] != NULL 
		&& (lastMovementSnapshot.sequence - lastPositionSequence) < 500)
	{
		localSequence++;
		playerList[1]->input.W = lastMovementSnapshot.forward;
		playerList[1]->input.S = lastMovementSnapshot.backward;
		playerList[1]->input.A = lastMovementSnapshot.left;
		playerList[1]->input.D = lastMovementSnapshot.right;
		playerList[1]->input.E = lastMovementSnapshot.interact;
		playerList[1]->input.R = lastMovementSnapshot.place;
		playerList[1]->input.SHIFT = lastMovementSnapshot.sprint;

		playerList[1]->position.y = DequantizeFloat(lastMovementSnapshot.y, MAX_BOUNDS);
		playerList[1]->pitch = DequantizeFloat(lastMovementSnapshot.pitch, MAX_BOUNDS);
		playerList[1]->yaw = DequantizeFloat(lastMovementSnapshot.yaw, MAX_BOUNDS);
	}

	// Send Movement data packet
	uint8_t buffer[1 + sizeof(struct MovementSnapshot)];
	buffer[0] = MovementSnapshot; // Set message type
	memcpy(buffer + 1, &movementSnapshot, sizeof(struct MovementSnapshot));
	SendPlayerData(buffer, sizeof(buffer), isServer);

	if (isServer)
		SessionManager_Server_SendPackets();
	else
		SessionManager_Client_SendPackets();
}

// Network correction tick - runs occasionally to correct desynchronization
void NetworkCorrectionTick(bool isServer)
{
	struct PositionSnapshot positionSnapshot;
	positionSnapshot.sequence = localSequence;
	positionSnapshot.posX = QuantizeFloat(playerList[0]->position.x, MAX_BOUNDS);
	positionSnapshot.posY = QuantizeFloat(playerList[0]->position.y, MAX_BOUNDS);
	positionSnapshot.posZ = QuantizeFloat(playerList[0]->position.z, MAX_BOUNDS);
	positionSnapshot.yaw = QuantizeFloat(playerList[0]->yaw, MAX_BOUNDS);
	positionSnapshot.pitch = QuantizeFloat(playerList[0]->pitch, MAX_BOUNDS);

	// Send position correction packet
	uint8_t buffer[1 + sizeof(struct PositionSnapshot)];
	buffer[0] = PositionSnapshot; // Set message type
	memcpy(buffer + 1, &positionSnapshot, sizeof(struct PositionSnapshot));
	SendPlayerData(buffer, sizeof(buffer), isServer);

	if (isServer)
		SessionManager_Server_SendPackets();
	else
		SessionManager_Client_SendPackets();
}

// Stores float into a quantized int16_t based on max absolute value
int16_t QuantizeFloat(float value, float max_abs) 
{
	float scaled = (value / max_abs) * 32767.0f;

	if (scaled > 32767.0f) 
		scaled = 32767.0f;
	else if (scaled < -32767.0f) 
		scaled = -32767.0f;

	return (int16_t)lroundf(scaled);
}

// Retrieves float from quantized int16_t based on max absolute value
float DequantizeFloat(int16_t scaledValue, float max_abs) 
{
	return (scaledValue / 32767.0f) * max_abs;
}