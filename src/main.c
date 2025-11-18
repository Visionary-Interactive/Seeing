#include "includes.h"
#include "player.h"
#include "object.h"
#include "camera.h"
#include "impairment.h"
#include "map.h"
#include "SessionManager.h"
#include "SessionStateController.h"

#define SERVER_PORT 12345

static void DrawLevel(void);

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
	playerList[0] = GetPlayer();
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
	SessionStateController_Init();
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

	while (!WindowShouldClose())
	{
		for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
		{
			SetPlayer(playerList[i]);
			UpdatePlayer(gameObjects);
		}
		UpdateImpairment(astig);
		SessionStateController_Tick(isServer);
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
	for (int i = 0; i < clientPlayerCount + 1; i++)
		RL_FREE(playerList[i]);

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