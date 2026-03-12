#include "render.h"
#include "player.h"
#include "camera.h"
#include "prop.h"
#include "lens.h"
#include "impairment.h"
#include "menu.h"
#include "skybox.h"

static Impairment* astigmatism = NULL;
static Impairment* tritanopia = NULL;
static Impairment* convex = NULL;
static Impairment* glaucoma = NULL;

void InitSceneImpairments(int screenWidth, int screenHeight)
{
	if (astigmatism != NULL) return;

	astigmatism = LoadImpairment(Astigmatism, screenWidth, screenHeight);
	tritanopia = LoadImpairment(Tritanopia, screenWidth, screenHeight);
	convex = LoadImpairment(Convex, screenWidth, screenHeight);
	glaucoma = LoadImpairment(Glaucoma, screenWidth, screenHeight);
}

static void UpdateSceneImpairments(void)
{
	if (tritanopia) UpdateImpairment(tritanopia);
	if (astigmatism) UpdateImpairment(astigmatism);
	if (glaucoma) UpdateImpairment(glaucoma);
	if (convex) UpdateImpairment(convex);
}

void DestroySceneImpairments(void)
{
	if (astigmatism) { DestroyImpairment(astigmatism); astigmatism = NULL; }
	if (tritanopia) { DestroyImpairment(tritanopia); tritanopia = NULL; }
	if (convex) { DestroyImpairment(convex); convex = NULL; }
	if (glaucoma) { DestroyImpairment(glaucoma); glaucoma = NULL; }
}

void RenderSceneToTexture(MenuScreen currentScreen, RenderTexture2D sceneColorRT, Camera* camera, Props* props, ParticlePool* pool)
{
	float dt = GetFrameTime();
	UpdateParticleEmitter(dt);

    if (currentScreen == menu_game || currentScreen == menu_game_paused || currentScreen == menu_editor)
    {
        BeginTextureMode(sceneColorRT);
        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);
			rlDisableBackfaceCulling();
			rlDisableDepthMask();
			DrawModel(GetSkybox()->model, camera->position, 1.0f, WHITE);
			rlEnableBackfaceCulling();
			rlEnableDepthMask();
			DrawFloor((Vector3) { 0.0f, 0.0f, 0.0f }, WHITE);
			
			// Draw players
			for (int i = 0; i < clientPlayerCount + 1; i++)
				RenderPlayer(playerList[i], props);

            RenderProps(props);
			RenderParticlePool(pool, *camera, PURPLE);
        EndMode3D();
        EndTextureMode();

        if ((currentScreen == menu_game && IsKeyPressed(KEY_ESCAPE)) || (currentScreen == menu_editor && IsKeyPressed(KEY_ESCAPE))) SetCurrentScreen(menu_game_paused);
        return;
    }

    BeginTextureMode(sceneColorRT);
    ClearBackground(BLACK);
    EndTextureMode();
}

void RenderFinalFrame(MenuScreen currentScreen, RenderTexture2D sceneColorRT, Camera* camera, Props* props, int swap, int screenWidth, int screenHeight)
{
	BeginDrawing();
	ClearBackground(WHITE);

	if (currentScreen == menu_game || currentScreen == menu_game_paused || currentScreen == menu_editor)
	{
		UpdateSceneImpairments();

		if (swap == 0 && tritanopia) BeginImpairment(tritanopia);
		else if (swap == 1 && glaucoma) BeginImpairment(glaucoma);
		else if (swap == 2 && astigmatism) BeginImpairment(astigmatism);

		Rectangle src = { 0, 0, (float)sceneColorRT.texture.width, -(float)sceneColorRT.texture.height };
		Rectangle dst = { 0, 0, (float)screenWidth, (float)screenHeight };
		DrawTexturePro(sceneColorRT.texture, src, dst, (Vector2) { 0, 0 }, 0.0f, WHITE);

		UpdateLensShader(camera, sceneColorRT);
		rlDisableDepthMask();
		BeginMode3D(*camera);
		RenderLensProps(props);
		EndMode3D();
		rlEnableDepthMask();

		if (swap == 0 && tritanopia) EndImpairment(tritanopia);
		else if (swap == 1 && glaucoma) EndImpairment(glaucoma);
		else if (swap == 2 && astigmatism) EndImpairment(astigmatism);

		for (int i = 0; i < clientPlayerCount + 1; i++) // Update for all players
		{
			if (playerList[i] == NULL) continue;
			SetPlayer(playerList[i]);
			UpdateInteractions(props);
		}

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
		DrawUI(playerList[0]->inventory, playerList[0]->selectedSlot);
	}

	if (currentScreen != menu_game)
	{
		DrawMenu();
	}

	EndDrawing();
}