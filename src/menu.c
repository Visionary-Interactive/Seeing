#pragma once

#include "menu.h"
#include "includes.h"

Button playButton = { { 100, 100, 200, 50 }, "Play" };
Button saveButton = { { 100, 200, 200, 50 }, "Save/Load" };
Button exitButton = { { 100, 400, 200, 50 }, "Exit Game" };
Button optionsButton = { { 100, 300, 200, 50 }, "Options" };
Button menuButton = { { 100, 200, 200, 50 }, "Back to Menu" };
Button levelButton = { { 100, 300, 200, 50 }, "Level Select" };


static MenuScreen currentScreen = menu_main;
static MenuScreen lastScreen;


void DrawMenu()
{
    MenuScreen screen = GetCurrentScreen();

    switch (screen)
    {
    case menu_main:
        DrawText("A Game about Seeing", 100, 40, 30, BLACK);
        DrawButton(playButton, DARKGRAY);
		DrawButton(saveButton, DARKGRAY);
        DrawButton(optionsButton, DARKGRAY);
        DrawButton(exitButton, DARKGRAY);
        break;

    case menu_game:
        break;

    case menu_options:
        DrawText("OPTIONS", 100, 40, 30, ORANGE);
		DrawButton(menuButton, DARKGRAY);
        break;

    case menu_level_select:
       DrawText("LEVEL SELECT", 100, 40, 30, GREEN);
       DrawButton(menuButton, DARKGRAY);
		break;

    case menu_multi:
        DrawText("MULTIPLAYER", 100, 40, 30, BLUE);
        DrawButton(menuButton, DARKGRAY);
		break;

    case menu_game_paused:
        DrawText("GAME PAUSED", 100, 40, 30, MAGENTA);
		break;


    default:
        DrawText("UNKNOWN SCREEN", 100, 40, 30, RED);
        break;
    }
}

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
            if (strcmp(button.text, "Play") == 0)
                SetCurrentScreen(menu_game);

            if (strcmp(button.text, "Options") == 0)
                SetCurrentScreen(menu_options);

            if (strcmp(button.text, "Exit Game") == 0)
				CloseWindow();
           
            if (strcmp(button.text, "Back to Menu") == 0)
				SetCurrentScreen(menu_main);
        }
    }
}


MenuScreen GetCurrentScreen() {
    return currentScreen;
}

void SetCurrentScreen(MenuScreen newScreen) {
    currentScreen = newScreen;
    static MenuScreen lastScreen = menu_main;

    if (currentScreen != lastScreen)
    {
        if (currentScreen == menu_game) DisableCursor();
        else EnableCursor();
        lastScreen = currentScreen;
    }
}