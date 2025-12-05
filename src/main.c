#include "includes.h"
#include "player.h"
#include "prop.h"
#include "camera.h"
#include "impairment.h"
#include "map.h"
#include "menu.h"
#include "sessionManager.h"
#include "sessionStateController.h"
#include "lens.h"


int main(int argc, char** argv)
{

	const int screenWidth = 1600;
	const int screenHeight = 900;

	playerColor = RED;
	remoteColor = BLACK;

	//all this stuff should be toggleable
	//SetConfigFlags(FLAG_MSAA_4X_HINT);

	InitWindow(screenWidth, screenHeight, "A Game About Seeing");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);
	//DisableCursor();

	InitPlayer();
	playerList[0] = GetPlayer();

	InitCamera();
	Camera* camera = GetCamera();

	CreatePropStructure();
	Props* props = GetPropStructure();

	RenderTexture2D sceneColorRT = LoadRenderTexture(screenWidth, screenHeight);
	InitLensShader(screenWidth, screenHeight, sceneColorRT);

	LoadPropTest(props);
	//Map gameMap;

	Impairment* astig = LoadImpairment(Astigmatism, screenWidth, screenHeight);
	Impairment* tritan = LoadImpairment(Tritanopia, screenWidth, screenHeight);
	Impairment* convex = LoadImpairment(Convex, screenWidth, screenHeight);

	SessionManager_Init();
	SessionStateController_Init();

	// Set up Server/Client
	bool isServer = false;
	if (argc < 2)
	{
		printf("No CLI argument provided for <server|client>, defaulting to server\n");
		isServer = true;
		SessionManager_CreateServer("UDP", SERVER_PORT);
	}
	else
	{
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
		else
		{
			printf("Invalid CLI argument! It's hosed. Aborting...\n");
			return 1;
		}
	}

	while (!WindowShouldClose())
	{
		MenuScreen currentScreen = GetCurrentScreen();

		if (currentScreen == menu_game)
		{
			if (playerList[i] == NULL) continue;
			SetPlayer(playerList[i]);
			UpdatePlayer(props);
		}

		UpdateImpairment(astig);
		//UpdateImpairment(convex);
		SessionStateController_Tick(isServer);
		RefreshCamera(playerList[0]);

		BeginTextureMode(sceneColorRT);
        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);
        DrawFloor();
        DrawModel(playerList[0]->model, playerList[0]->position, 1.0f, playerColor);
		// Draw remote player
		if (clientPlayerCount == 1) DrawModel(playerList[1]->model, playerList[1]->position, 1.0f, remoteColor);
		RenderProps(props);
        EndMode3D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

		BeginImpairment(astig);

		Rectangle src = { 0, 0, (float)sceneColorRT.texture.width, -(float)sceneColorRT.texture.height };
		Rectangle dst = { 0, 0, (float)screenWidth, (float)screenHeight };
		DrawTexturePro(sceneColorRT.texture, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);

		UpdateLensShaderPerFrame(camera, sceneColorRT);
		rlDisableDepthMask();
		BeginMode3D(*camera);
			RenderLensProps(props);
		EndMode3D();
		rlEnableDepthMask();

		EndImpairment(astig);

		for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
		{
			if (playerList[i] == NULL) continue;
			SetPlayer(playerList[i]);
			UpdateInteractions(props);
		}

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

	UnloadRenderTexture(sceneColorRT);

	DestroyImpairment(astig);
	DestroyImpairment(tritan);
	DestroyImpairment(convex);
	DestroyCamera();
	for (int i = 0; i < clientPlayerCount + 1; i++) {
		if (playerList[i] == NULL) continue;
		RL_FREE(playerList[i]);
	}

	// Stop Server/Client
	if (argc >= 2)
	{
		if (strcmp(argv[1], "server") == 0) SessionManager_StopServer();
		else if (strcmp(argv[1], "client") == 0) SessionManager_StopClient();
	}

	CloseWindow();
	free(props);
	return 0;
}


// CMD build and start debugging
// cd build && ./premake5 gmake && cd .. && make && ./bin/Debug/Seeing