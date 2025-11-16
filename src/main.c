#include "includes.h"
#include "player.h"
#include "object.h"
#include "camera.h"
#include "impairment.h"
#include "map.h"
#include "SessionManager.h"
#include <time.h>

#define SERVER_PORT 12345
#define TICK_RATE_MS 20
#define MAX_PLAYERS 2


static void DrawLevel(void);

static Player* playerList[MAX_PLAYERS];
static int clientPlayerCount = 0;
Color playerColor, remoteColor;

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
			SendIncomingPlayer(NULL, &incomingPlayer, false);
		break;
	}
	case 3: // NBN_CLIENT_DISCONNECTED / NBN_DISCONNECTED
	{
		printf("A player has disconnected.\n");
		RL_FREE(playerList[clientPlayerCount]);
		clientPlayerCount--;
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

int main(int argc, char** argv)
{
	const int screenWidth = 1600;
	const int screenHeight = 900;

	InitWindow(screenWidth, screenHeight, "A Game About Seeing");

	//all this stuff should be toggleable
	DisableCursor(); //hide cursor for mouselook
	SetTargetFPS(120);
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	InitPlayer();
	playerList[clientPlayerCount] = GetPlayer();
	playerColor = RED;
	remoteColor = BLACK;

	Model doorModel = LoadModelFromMesh(GenMeshCube(3.0, 4.0, 0.5));

	//GameObject array to hold all objects in the scene
	GameObject* gameObjects = (GameObject*)malloc(sizeof(GameObject));
	memset(gameObjects, 0, sizeof(GameObject));

	//Create a Wall to test collision
	CreateObject(gameObjects,
		(Vector3) {
		0, 4.0, -7
	},
		(Vector3) {
		1.0, 1.0, 1.0
	},
		LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), RED, OBJECT_VISIBILE | OBJECT_COLLIDER);


	//Create the door object to interact with
	int doorID = CreateObject(gameObjects,
		(Vector3) {
		0, 2.0f, -10.0f
	},
		(Vector3) {
		3.0f, 4.0f, 0.5f
	}, doorModel, GREEN, OBJECT_VISIBILE | OBJECT_COLLIDER | OBJECT_INTERACTABLE | OBJECT_DOOR);
	gameObjects->interactType[doorID] = INTERACTABLE_DOOR;

	int pickupID = CreateObject(gameObjects,
		(Vector3) {
		-10, 4.0, 0
	},
		(Vector3) {
		1.0, 1.0, 1.0
	},
		LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, OBJECT_VISIBILE | OBJECT_COLLIDER | OBJECT_INTERACTABLE | OBJECT_PICKUP);
	gameObjects->interactType[pickupID] = INTERACTABLE_PICKUP;

	InitCamera();
	Camera* camera = GetCamera();

	Map gameMap;

	Impairment* astig = LoadImpairment(Astigmatism, screenWidth, screenHeight);

	// Set up Server/Client
	bool isServer = false;
	if (argc < 2)
	{
		printf("Usage: %s <server|client> [host-for-client]\n", argv[0]);
		return 1;
	}

	SessionManager_Init();
	if (strcmp(argv[1], "server") == 0)
	{
		isServer = true;
		SessionManager_CreateServer("UDP", SERVER_PORT);
	}
	else if (strcmp(argv[1], "client") == 0)
	{
		playerColor = BLUE;
		const char* host = (argc >= 3) ? argv[2] : "127.0.0.1"; // set default host to localhost
		SessionManager_CreateClient("UDP", host, SERVER_PORT);
	}

	CreatePlayer = CreateNewPlayer;
	InitalizeRemotePlayer = InitRemotePlayer;

	clock_t last_network_tick = clock();

	while (!WindowShouldClose())
	{
		for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
		{
			SetPlayer(playerList[i]);
			UpdatePlayer(gameObjects);
		}
		UpdateImpairment(astig);

		// Handle networking events at fixed tick rate
		clock_t now = clock();
		int elapsed_ms = (int)(now - last_network_tick);

		if (elapsed_ms >= TICK_RATE_MS)
		{
			last_network_tick = now;
			NetworkTick(isServer);
		}

		RefreshCamera(playerList[0]);

		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginImpairment(astig);

		BeginMode3D(*camera);
		DrawMap(&gameMap);
		// Draw Player
		DrawModel(playerList[0]->model, playerList[0]->position, 1.0f, playerColor);
		// Draw remote player
		if (clientPlayerCount == 1)
			DrawModel(playerList[1]->model, playerList[1]->position, 1.0f, remoteColor);
		RenderProps(gameObjects);
		EndMode3D();

		for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
		{
			SetPlayer(playerList[i]);
			UpdateInteractions(gameObjects);
		}

		EndImpairment(astig);
		DrawText("WASD to move, MOUSE to look, ESC to quit", 10, 10, 20, DARKGRAY);
		DrawText("SHIFT to sprint, SPACE to jump", 10, 40, 20, DARKGRAY);
		DrawText("Current Impairment Loaded: Astigmatism", 10, 70, 20, DARKGRAY);
		DrawText("[LEFT/RIGHT] to adjust angle, [UP/DOWN] to adjust intensity, [O/P] to swap presets", 10, 100, 20, DARKGRAY);
		DrawText(TextFormat("Player Position: (%.3f, %.3f, %.3f)",
			playerList[0]->position.x, playerList[0]->position.y, playerList[0]->position.z),
			10, 130, 20, DARKGRAY);
		DrawText(TextFormat("Camera Target: (%.3f, %.3f, %.3f)",
			camera->target.x, camera->target.y, camera->target.z),
			10, 160, 20, DARKGRAY);
		EndDrawing();
	}

	DestroyImpairment(astig);
	DestroyCamera();
	RL_FREE(playerList[0]);
	RL_FREE(playerList[1]);

	// Stop Server/Client
	if (strcmp(argv[1], "server") == 0)
		SessionManager_StopServer();
	else if (strcmp(argv[1], "client") == 0)
		SessionManager_StopClient();

	CloseWindow();
	free(gameObjects);
	return 0;
}


// CMD build and start debugging
// cd build && ./premake5 gmake && cd .. && make && ./bin/Debug/Seeing