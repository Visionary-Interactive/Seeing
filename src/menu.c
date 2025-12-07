#include "menu.h"
#include "includes.h"

// Define buttons for various menu options
Button playButton = { { 100, 100, 200, 50 }, "Play" };
Button saveButton = { { 100, 200, 200, 50 }, "Save/Load" };
Button exitButton = { { 100, 400, 200, 50 }, "Exit Game" };
Button optionsButton = { { 100, 300, 200, 50 }, "Options" };
Button menuButton = { { 100, 400, 200, 50 }, "Back to Menu" };
Button levelButton = { { 100, 100, 200, 50 }, "Level Select" };
Button level1Button = { { 100, 100, 200, 50 }, "Level 1" };

static MenuScreen currentScreen = menu_main;
static MenuScreen lastScreen;
static bool gExitRequested = false;

bool IsExitRequested(void) {
    return gExitRequested;
}

void RequestExit(void) {
    gExitRequested = true;
}

// Draws the current menu screen based on the current state
void DrawMenu()
{
    MenuScreen screen = GetCurrentScreen();

    switch (screen)
    {
    case menu_main:
        DrawText("A Game about Seeing", 100, 40, 30, BLACK);
        DrawButton(levelButton, DARKGRAY);
		DrawButton(saveButton, DARKGRAY);
        DrawButton(optionsButton, DARKGRAY);
        DrawButton(exitButton, DARKGRAY);
        break;

    case menu_game:
        DrawUI();
        break;

    case menu_options:
        DrawText("OPTIONS", 100, 40, 30, ORANGE);
		DrawButton(menuButton, DARKGRAY);
        break;

    case menu_level_select:
       DrawText("LEVEL SELECT", 100, 40, 30, GREEN);
       DrawButton(menuButton, DARKGRAY);
	   DrawButton(level1Button, DARKGRAY);
		break;

    case menu_multi:
        DrawText("MULTIPLAYER", 100, 40, 30, BLUE);
        DrawButton(menuButton, DARKGRAY);
		break;

    case menu_save:
        DrawText("SAVE/LOAD", 100, 40, 30, PURPLE);
		DrawButton(menuButton, DARKGRAY);
        break;

    case menu_game_paused:
		DrawButton(playButton, DARKGRAY);
		DrawButton(saveButton, DARKGRAY);
		DrawButton(optionsButton, DARKGRAY);
		DrawButton(menuButton, DARKGRAY);

        break;


    default:
        DrawText("UNKNOWN SCREEN", 100, 40, 30, RED);
        break;
    }
}

//Draws a button and handles its interaction logic
void DrawButton(Button button, Color color)
{
    DrawRectangleRec(button.bounds, color);
    DrawText(button.text, button.bounds.x + 10, button.bounds.y + 10, 20, WHITE);

    // Button press logic:
    if (CheckCollisionPointRec(GetMousePosition(), button.bounds))
    {
        DrawRectangleLines(button.bounds.x, button.bounds.y, button.bounds.width, button.bounds.height, YELLOW);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Buttons trigger screen changes:
            if (strcmp(button.text, "Play" )== 0)
                SetCurrentScreen(menu_game);

            if (strcmp(button.text, "Level 1") == 0)
                SetCurrentScreen(menu_game);

            if (strcmp(button.text, "Options") == 0)
                SetCurrentScreen(menu_options);

            if (strcmp(button.text, "Exit Game") == 0)
                RequestExit();
            if (strcmp(button.text, "Back to Menu") == 0)
				SetCurrentScreen(menu_main);
			if (strcmp(button.text, "Level Select") == 0)
				SetCurrentScreen(menu_level_select);
            if (strcmp(button.text, "Save/Load") == 0)
				SetCurrentScreen(menu_save);
        }
    }
}

//will draw interaction to the UI for the player
void DrawUI()
{
    if (uiInteraction == true)
    {
        DrawRectangle(160, 840, 75, 75, BLUE);
    }
}


// Returns the current menu screen
MenuScreen GetCurrentScreen() {
    return currentScreen;
}

// Sets the current menu screen and manages cursor visibility
void SetCurrentScreen(MenuScreen newScreen) {
    currentScreen = newScreen;
    lastScreen = menu_main;

    if (currentScreen != lastScreen)
    {
        if (currentScreen == menu_game) DisableCursor();
        else EnableCursor();
        lastScreen = currentScreen;
    }
}

// Sets whether the UI is currently being interacted with
void SetUIInteraction(bool interaction)
{
	uiInteraction = interaction;
	printf("UI Interaction set to: %s\n", interaction ? "true" : "false");
}

