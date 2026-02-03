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
#include "render.h"

int main(int argc, char** argv)
{
	const int screenWidth = 1600;
	const int screenHeight = 900;

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
	InitMap(&gameMap, "resources/maps/pz_1");
	InitSaveSlots();
	LoadPropTest(props);
	//LoadMapFile(&gameMap, "resources/maps/pz_1");

	Impairment* astig = LoadImpairment(Astigmatism, screenWidth, screenHeight);
	Impairment* tritan = LoadImpairment(Tritanopia, screenWidth, screenHeight);
	Impairment* convex = LoadImpairment(Convex, screenWidth, screenHeight);
	Impairment* glau = LoadImpairment(Glaucoma, screenWidth, screenHeight);

	SessionManager_Init();
	SessionStateController_Init();

	bool swap = false;

	while (!WindowShouldClose() && !IsExitRequested())
	{
		if (IsKeyDown(KEY_LEFT_BRACKET)) swap = false;
        if (IsKeyDown(KEY_RIGHT_BRACKET)) swap = true;

		MenuScreen currentScreen = GetCurrentScreen();
		if (multiplayerSession) SessionStateController_Tick(isServer); // Networking tick
		if (IsKeyPressed(KEY_ESCAPE) && currentScreen == menu_game_paused) { SetCurrentScreen(menu_game); }

		if (currentScreen == menu_game || currentScreen == menu_editor)
		{
			for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
			{
				if (playerList[i] == NULL) continue;
				SetPlayer(playerList[i]);
				UpdatePlayer(props);
			}

			RefreshCamera(playerList[0]);
		}

		RenderSceneToTexture(currentScreen, sceneColorRT, camera, props);
		RenderFinalFrame(currentScreen, sceneColorRT, camera, props, swap, screenWidth, screenHeight);
	}

	SaveMap(&gameMap, "resources/maps/pz_1");

	UnloadRenderTexture(sceneColorRT);
	
	DestroyCamera();

	DestroyProps(props);

	for (int i = 0; i < clientPlayerCount; i++) {
		if (playerList[i] == NULL) continue;
		RL_FREE(playerList[i]);
	}

	// Stop Server/Client //ask alice about this
	//if (isServer) SessionManager_StopServer();
	//else SessionManager_StopClient();

	CloseWindow();
	return 0;
}


// CMD build and start debugging
// cd build && ./premake5 gmake && cd .. && make && ./bin/Debug/Seeing