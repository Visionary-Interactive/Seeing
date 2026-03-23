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
#include "skybox.h"
#include "particleEmitter.h"
#include "sound.h"
#include "modelPool.h"

int main(int argc, char** argv)
{
	const int screenWidth = 1600;
	const int screenHeight = 900;

	//SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetMasterVolume(0.0f);
	InitWindow(screenWidth, screenHeight, "A Game About Seeing");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	LoadMenuElements();

	InitModelCache();
	InitSoundSystem();

	InitPlayer();
	playerList[0] = GetPlayer();

	InitCamera();
	Camera* camera = GetCamera();

	InitSkybox("resources/maps/pz_1", false);
    Skybox* skybox = GetSkybox();

	InitFloor("resources/global/models/sandfloor/sand_diff.jpg", 128.0f, 128.0f, 32.0f);
	Floor* floor = GetFloor();

	CreatePropStructure();
	Props* props = GetPropStructure();

	RenderTexture2D sceneColorRT = LoadRenderTexture(screenWidth, screenHeight);
	InitLensShader(screenWidth, screenHeight, sceneColorRT);
  
	InitSceneImpairments(screenWidth, screenHeight);

	Map gameMap;
	InitMap(&gameMap, "resources/maps/pz_1");

	//InitSaveSlots();
	LoadPropTest(props);
	//LoadLevel2(props);
	//LoadMapFile(&gameMap, "resources/maps/pz_1");
	//LoadMapProgress(&gameMap, playerList[0], "resources/maps/pz_1");

	SessionManager_Init();
	SessionStateController_Init();

	int swap = 0;

	float intensity = 0;

	while (!WindowShouldClose() && !IsExitRequested())
	{
		float dt = GetFrameTime();
		if (IsKeyDown(KEY_LEFT_BRACKET)) swap = 0;
        if (IsKeyDown(KEY_RIGHT_BRACKET)) swap = 1;
		if (IsKeyDown(KEY_APOSTROPHE)) swap = 2;

		if (playerList[0]->pendingImpairment != -1)
		{
			swap = playerList[0]->pendingImpairment;

			intensity = playerList[0]->pendingIntensity;

			playerList[0]->activeImpairmentIntensity = intensity; // store current

			playerList[0]->pendingImpairment = -1; // reset ONLY pending

			

		}

		MenuScreen currentScreen = GetCurrentScreen();
		if (multiplayerSession) SessionStateController_Tick(isServer); // Networking tick
		if (IsKeyPressed(KEY_ESCAPE) && currentScreen == menu_game_paused) { SetCurrentScreen(menu_game); }

		if (currentScreen == menu_game || currentScreen == menu_editor)
		{
			for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
			{
				UpdateTextBox(dt);
				if (playerList[i] == NULL) continue;
				SetPlayer(playerList[i]);
				UpdatePlayer(props);
			}

			RefreshCamera(playerList[0]);
		}

		RenderSceneToTexture(currentScreen, sceneColorRT, camera, props, GetParticlePool());
		RenderFinalFrame(currentScreen, sceneColorRT, camera, props, swap, intensity,screenWidth, screenHeight);
	}

	SaveMapFile(&gameMap, "resources/maps/pz_1");
	//SaveMapProgress(&gameMap, playerList[0], "resources/maps/pz_1");

	UnloadRenderTexture(sceneColorRT);
	
	DestroyCamera();
	DestroyParticlePool(GetParticlePool());
	DestroyProps(props);

	for (int i = 0; i < clientPlayerCount; i++) {
		if (playerList[i] == NULL) continue;
		RL_FREE(playerList[i]);
	}

	DestroySoundSystem();

	CloseWindow();
	return 0;
}


// CMD build and start debugging
// cd build && ./premake5 gmake && cd .. && make && ./bin/Debug/Seeing