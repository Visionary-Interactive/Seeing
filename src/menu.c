#pragma once

#include "menu.h"
#include "includes.h"
#include "sessionManager.h"
#include "sessionStateController.h"

Button playButton = { { 100, 100, 200, 50 }, "Play" };
Button saveButton = { { 100, 200, 200, 50 }, "Save/Load" };
Button levelButton = { { 100, 300, 200, 50 }, "Level Select" };
Button multiMenuButton = { { 100, 400, 200, 50 }, "Multiplayer" };
Button optionsButton = { { 100, 500, 200, 50 }, "Options" };
Button exitButton = { { 100, 600, 200, 50 }, "Exit Game" };
Button menuButton = { { 100, 600, 200, 50 }, "Back to Menu" };
Button multiHostButton = { { 100, 100, 200, 50 }, "Host Game" };
Button multiJoinButton = { { 100, 200, 200, 50 }, "Join Game" };

Rectangle ipAddressText = { 400, 200, 200, 50 };
char ipAddress[32] = "127.0.0.1";
bool ipBoxFocused = false;

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
		DrawButton(levelButton, DARKGRAY);
		DrawButton(multiMenuButton, DARKGRAY);
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
        DrawButton(multiHostButton, DARKGRAY);
        DrawButton(multiJoinButton, DARKGRAY);
        DrawTextBox(ipAddressText, ipAddress, strlen(ipAddress), 32, &ipBoxFocused);
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
            else if (strcmp(button.text, "Options") == 0)
                SetCurrentScreen(menu_options);
            else if (strcmp(button.text, "Exit Game") == 0)
                CloseWindow();
            else if (strcmp(button.text, "Back to Menu") == 0)
                SetCurrentScreen(menu_main);
            else if (strcmp(button.text, "Multiplayer") == 0)
                SetCurrentScreen(menu_multi);
            else if (strcmp(button.text, "Host Game") == 0)
            {
                isServer = true;
                SessionManager_CreateServer("UDP", SERVER_PORT);
                multiplayerSession = true;
                SetCurrentScreen(menu_game);
            }
            else if (strcmp(button.text, "Join Game") == 0)
            {
                playerColor = BLUE;
                const char* host = ipAddress;
                SessionManager_CreateClient("UDP", host, SERVER_PORT);
                multiplayerSession = true;
                SetCurrentScreen(menu_game);
            }
        }
    }
}

void DrawTextBox(Rectangle bounds, char* buffer, int currentSize, int maxSize, bool* focused)
{
    DrawRectangleRec(bounds, LIGHTGRAY);
    DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height, (*focused) ? BLUE : DARKGRAY);
    DrawText(buffer, bounds.x + 5, bounds.y + 10, 20, BLACK);

    if (CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        *focused = true;
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(GetMousePosition(), bounds))
        *focused = false;

    // Handle keyboard input if focused
    if (*focused)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (currentSize < maxSize - 1))
            {
                buffer[currentSize] = (char)key;
                buffer[currentSize + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyDown(KEY_BACKSPACE)) && currentSize > 0)
        {
            buffer[currentSize - 1] = '\0';
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