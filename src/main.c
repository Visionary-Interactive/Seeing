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

int main(int argc, char** argv)
{
	const int screenWidth = 1600;
	const int screenHeight = 900;

	//SetConfigFlags(FLAG_MSAA_4X_HINT);

	InitWindow(screenWidth, screenHeight, "A Game About Seeing");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

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

	ParticlePool* pool = InitParticlePool(512);
	InitFlameTemplate();
	ParticleTemplate* template = GetParticleTemplate();
	InitParticleEmitter(pool, 20.0f, (Vector3){0.0f, 0.0f, 0.0f}, template, RED);
	InitParticleEmitter(pool, 20.0f, (Vector3){10.0f, 0.0f, 0.0f}, template, WHITE);
	InitParticleEmitter(pool, 20.0f, (Vector3){20.0f, 0.0f, 0.0f}, template, YELLOW);
	InitParticleEmitter(pool, 20.0f, (Vector3){30.0f, 0.0f, 0.0f}, template, GREEN);
	InitParticleEmitter(pool, 20.0f, (Vector3){40.0f, 0.0f, 0.0f}, template, BLUE);

	RenderTexture2D sceneColorRT = LoadRenderTexture(screenWidth, screenHeight);
	InitLensShader(screenWidth, screenHeight, sceneColorRT);
  
	InitSceneImpairments(screenWidth, screenHeight);

	Map gameMap;
	InitMap(&gameMap, "resources/maps/pz_1");
	InitSaveSlots();
	LoadPropTest(props);
	//LoadMapFile(&gameMap, "resources/maps/pz_1");
	//LoadMapProgress(&gameMap, playerList[0], "resources/maps/pz_1");

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

		RenderSceneToTexture(currentScreen, sceneColorRT, camera, props, pool);
		RenderFinalFrame(currentScreen, sceneColorRT, camera, props, swap, screenWidth, screenHeight);
	}

	//SaveMapProgress(&gameMap, playerList[0], "resources/maps/pz_1");

	UnloadRenderTexture(sceneColorRT);
	
	DestroyCamera();
	DestroyParticlePool(pool);
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