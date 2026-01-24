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
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("Entered main at: %s\n", cwd);

	const int screenWidth = 1600;
	const int screenHeight = 900;

	bool swap = false;

	//all this stuff should be toggleable
	//SetConfigFlags(FLAG_MSAA_4X_HINT);

	InitWindow(screenWidth, screenHeight, "A Game About Seeing");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	InitPlayer();
	playerList[0] = GetPlayer();

	InitCamera();
	Camera* camera = GetCamera();

	CreatePropStructure();
	Props* props = GetPropStructure();

	RenderTexture2D sceneColorRT = LoadRenderTexture(screenWidth, screenHeight);
	InitLensShader(screenWidth, screenHeight, sceneColorRT);

	Map gameMap;
	InitMap(&gameMap, "maps/pz_1");
	//LoadPropTest(props);
	LoadMapFile("maps/pz_1");

	Impairment* astig = LoadImpairment(Astigmatism, screenWidth, screenHeight);
	Impairment* tritan = LoadImpairment(Tritanopia, screenWidth, screenHeight);
	Impairment* convex = LoadImpairment(Convex, screenWidth, screenHeight);
	Impairment* glau = LoadImpairment(Glaucoma, screenWidth, screenHeight);

	SessionManager_Init();
	SessionStateController_Init();

	while (!WindowShouldClose() && !IsExitRequested())
	{
		if (IsKeyDown(KEY_LEFT_BRACKET)) swap = false;
        if (IsKeyDown(KEY_RIGHT_BRACKET)) swap = true;

		MenuScreen currentScreen = GetCurrentScreen();
		if (multiplayerSession) SessionStateController_Tick(isServer); // Networking tick
		if (IsKeyPressed(KEY_ESCAPE) && currentScreen == menu_game_paused) { SetCurrentScreen(menu_game); }

		if (currentScreen == menu_game)
		{
			for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
			{
				if (playerList[i] == NULL) continue;
				SetPlayer(playerList[i]);
				UpdatePlayer(props);
			}

			UpdateImpairment(tritan);
			UpdateImpairment(astig);
			UpdateImpairment(glau);
			RefreshCamera(playerList[0]);

			BeginTextureMode(sceneColorRT);
			ClearBackground(RAYWHITE);

			BeginMode3D(*camera);
			DrawFloor();
			DrawModel(playerList[0]->model, playerList[0]->position, 1.0f, playerColor);
			// Draw remote player
			if (clientPlayerCount == 1) DrawModel(playerList[1]->model, playerList[1]->position, 1.0f, remoteColor);
			RenderProps(props);
			if (IsKeyPressed(KEY_ESCAPE) && currentScreen == menu_game) { SetCurrentScreen(menu_game_paused); }
		}

        EndMode3D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(WHITE);

		if (currentScreen == menu_game || currentScreen == menu_game_paused)
		{
			if (swap) BeginImpairment(tritan);
			else BeginImpairment(glau);

			Rectangle src = { 0, 0, (float)sceneColorRT.texture.width, -(float)sceneColorRT.texture.height };
			Rectangle dst = { 0, 0, (float)screenWidth, (float)screenHeight };
			DrawTexturePro(sceneColorRT.texture, src, dst, (Vector2) { 0, 0 }, 0.0f, WHITE);

			UpdateLensShaderPerFrame(camera, sceneColorRT);
			rlDisableDepthMask();
			BeginMode3D(*camera);
			RenderLensProps(props);
			EndMode3D();
			rlEnableDepthMask();

			if (swap) EndImpairment(tritan);
			else EndImpairment(glau);

			for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
			{
				if (playerList[i] == NULL) continue;
				SetPlayer(playerList[i]);
				UpdateInteractions(props);
			}

			//Drawing things that will play during the game
			DrawText("WASD to move, MOUSE to look, ESC to quit", 10, 10, 20, DARKGRAY);
			DrawText("SHIFT to sprint, SPACE to jump", 10, 40, 20, DARKGRAY);
			if (!swap) DrawText("Current Impairment Loaded: Astigmatism", 10, 70, 20, DARKGRAY);
			else DrawText("Current Impairment Loaded: Tritanopia", 10, 70, 20, DARKGRAY);
			DrawText("[LEFT/RIGHT] to adjust angle, [UP/DOWN] to adjust intensity, [O/P] to swap presets", 10, 100, 20, DARKGRAY);
			DrawText(TextFormat("Player Position: (%.3f, %.3f, %.3f)",
				playerList[0]->position.x, playerList[0]->position.y, playerList[0]->position.z),
				10, 130, 20, DARKGRAY);
			DrawText(TextFormat("Camera Target: (%.3f, %.3f, %.3f)",
				camera->target.x, camera->target.y, camera->target.z),
				10, 160, 20, DARKGRAY);
			DrawRectangle(130, screenHeight - 60, screenWidth - 260, 120, DARKGRAY);
			DrawUI();
		}

		if (currentScreen != menu_game)
		{
			DrawMenu();
		}

		EndDrawing();
	}

	SaveMap(&gameMap, "maps/pz_1");

	UnloadRenderTexture(sceneColorRT);

	DestroyImpairment(astig);
	DestroyImpairment(tritan);
	DestroyImpairment(convex);
	DestroyImpairment(glau);
	DestroyCamera();

	for (int i = 0; i < clientPlayerCount; i++) {
		if (playerList[i] == NULL) continue;
		RL_FREE(playerList[i]);
	}

	// Stop Server/Client //ask alice about this
	//if (isServer) SessionManager_StopServer();
	//else SessionManager_StopClient();

	CloseWindow();
	free(props);
	return 0;
}


// CMD build and start debugging
// cd build && ./premake5 gmake && cd .. && make && ./bin/Debug/Seeing