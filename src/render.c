// render.c
#include "render.h"
#include "player.h"
#include "prop.h"
#include "lens.h"
#include "impairment.h"
#include "menu.h"

static Impairment* astigmatism = NULL;
static Impairment* tritanopia = NULL;
static Impairment* convex = NULL;
static Impairment* glaucoma = NULL;

static void UpdateSceneImpairments(void)
{
	if (tritanopia) UpdateImpairment(tritanopia);
	if (astigmatism) UpdateImpairment(astigmatism);
	if (glaucoma) UpdateImpairment(glaucoma);
	if (convex) UpdateImpairment(convex);
}

void InitSceneImpairments(int screenWidth, int screenHeight)
{
	if (astigmatism != NULL) return;

	astigmatism = LoadImpairment(Astigmatism, screenWidth, screenHeight);
	tritanopia = LoadImpairment(Tritanopia, screenWidth, screenHeight);
	convex = LoadImpairment(Convex, screenWidth, screenHeight);
	glaucoma = LoadImpairment(Glaucoma, screenWidth, screenHeight);
}

void DestroySceneImpairments(void)
{
	if (astigmatism) { DestroyImpairment(astigmatism); astigmatism = NULL; }
	if (tritanopia) { DestroyImpairment(tritanopia); tritanopia = NULL; }
	if (convex) { DestroyImpairment(convex); convex = NULL; }
	if (glaucoma) { DestroyImpairment(glaucoma); glaucoma = NULL; }
}

void RenderSceneToTexture(MenuScreen currentScreen, RenderTexture2D sceneColorRT, Camera* camera, Props* props)
{
    if (currentScreen == menu_game || currentScreen == menu_game_paused || currentScreen == menu_editor)
    {
        BeginTextureMode(sceneColorRT);
        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);
            DrawFloor();
            DrawModel(playerList[0]->model, playerList[0]->position, 1.0f, playerColor);
            if (clientPlayerCount == 1)
                DrawModel(playerList[1]->model, playerList[1]->position, 1.0f, remoteColor);
            RenderProps(props);
        EndMode3D();
        EndTextureMode();

        if ((currentScreen == menu_game && IsKeyPressed(KEY_ESCAPE)) || (currentScreen == menu_editor && IsKeyPressed(KEY_ESCAPE))) SetCurrentScreen(menu_game_paused);
        return;
    }

    BeginTextureMode(sceneColorRT);
    ClearBackground(BLACK);
    EndTextureMode();
}

void RenderFinalFrame(MenuScreen currentScreen, RenderTexture2D sceneColorRT, Camera* camera, Props* props, bool swap, int screenWidth, int screenHeight)
{
	BeginDrawing();
	ClearBackground(WHITE);

	if (currentScreen == menu_game || currentScreen == menu_game_paused || currentScreen == menu_editor)
	{
		UpdateSceneImpairments();

		if (swap && tritanopia) BeginImpairment(tritanopia);
		else if (!swap && glaucoma) BeginImpairment(glaucoma);

		Rectangle src = { 0, 0, (float)sceneColorRT.texture.width, -(float)sceneColorRT.texture.height };
		Rectangle dst = { 0, 0, (float)screenWidth, (float)screenHeight };
		DrawTexturePro(sceneColorRT.texture, src, dst, (Vector2) { 0, 0 }, 0.0f, WHITE);

		UpdateLensShader(camera, sceneColorRT);
		rlDisableDepthMask();
		BeginMode3D(*camera);
		RenderLensProps(props);
		EndMode3D();
		rlEnableDepthMask();

		if (swap && tritanopia) EndImpairment(tritanopia);
		else if (!swap && glaucoma) EndImpairment(glaucoma);

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
		DrawUI();
	}

	if (currentScreen != menu_game)
	{
		DrawMenu();
	}

	EndDrawing();
}