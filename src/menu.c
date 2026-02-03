#include "menu.h"
#include "includes.h"
#include "sessionManager.h"
#include "sessionStateController.h"
#include "sessionLobbyController.h"

// Define buttons for various menu options
Button playButton = { { 100, 100, 200, 50 }, "Play" };
Button saveButton = { { 100, 200, 200, 50 }, "Save/Load" };
Button levelButton = { { 100, 300, 200, 50 }, "Level Select" };
Button editorButton = { { 100, 400, 200, 50 }, "Level Editor" };
Button multiMenuButton = { { 100, 600, 200, 50 }, "Multiplayer" };
Button optionsButton = { { 100, 500, 200, 50 }, "Options" };
Button exitButton = { { 100, 700, 200, 50 }, "Exit Game" };
Button menuButton = { { 100, 600, 200, 50 }, "Back to Menu" };
Button multiConnectButton = { { 100, 200, 200, 50 }, "Connect" };
Button level1Button = { { 100, 100, 200, 50 }, "Level 1" };
#define SAVE_COLS 4
#define SAVE_ROWS 3
#define SAVE_SLOT_COUNT (SAVE_COLS * SAVE_ROWS)

Button saveSlots[SAVE_SLOT_COUNT];

Rectangle ipAddressText = { 400, 200, 200, 50 };
char ipAddress[32] = HOME_SERVER_IP;
bool ipBoxFocused = false;

static MenuScreen currentScreen = menu_main;
static MenuScreen lastScreen;
static bool gExitRequested = false;

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
		DrawButton(editorButton, DARKGRAY);
		DrawButton(multiMenuButton, DARKGRAY);
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

    case menu_editor:
        DrawUI();
        break;

    case menu_multi:
		DrawText("MULTIPLAYER", 100, 40, 30, BLUE);
		DrawTextBox(ipAddressText, ipAddress, strlen(ipAddress), 32, &ipBoxFocused);
		DrawButton(multiConnectButton, DARKGRAY);
        DrawButton(menuButton, DARKGRAY);
        break;

    case menu_multi2:
		DrawText("MULTIPLAYER", 100, 40, 30, BLUE);
		DrawTextBox(ipAddressText, ipAddress, strlen(ipAddress), 32, &ipBoxFocused);
		DrawText("Connecting to Game Server...", 100, 340, 30, DARKGRAY);
		if (SessionManager_Client_IsConnected())
		{
			DrawText("Connected to Game Server!", 100, 440, 30, GREEN);
            DrawText("Waiting for Lobby to Fill...", 100, 540, 30, DARKGRAY);
		}
		if (AssignMultiplayerStatus()) // get multiplayer status from lobby query
		{
			multiplayerSession = true;
			SetCurrentScreen(menu_game);
		}
		break;

    case menu_save:
        for (int i = 0; i < SAVE_SLOT_COUNT; i++)
        {
            DrawButton(saveSlots[i], DARKGRAY);
        }

        DrawButton(menuButton, DARKGRAY);
        break;

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
            if (strcmp(button.text, "Play") == 0)
                SetCurrentScreen(menu_game);
            else if (strcmp(button.text, "Level 1") == 0)
                SetCurrentScreen(menu_game);
            else if (strcmp(button.text, "Options") == 0)
                SetCurrentScreen(menu_options);
            else if (strcmp(button.text, "Level Editor") == 0)
                SetCurrentScreen(menu_editor);
            else if (strcmp(button.text, "Exit Game") == 0)
                RequestExit();
            else if (strcmp(button.text, "Back to Menu") == 0)
                SetCurrentScreen(menu_main);
            else if (strcmp(button.text, "Multiplayer") == 0)
                SetCurrentScreen(menu_multi);
            else if (strcmp(button.text, "Connect") == 0)
            {
                ConnectToHomeServer();
                SetCurrentScreen(menu_multi2);
            }
			else if (strcmp(button.text, "Level Select") == 0)
				SetCurrentScreen(menu_level_select);
            else if (strcmp(button.text, "Save/Load") == 0)
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

// Returns the current menu screen
MenuScreen GetCurrentScreen() {
    return currentScreen;
}

//sets up the save slots in the save/load menu
void InitSaveSlots()
{
    const int startX = 100;
    const int startY = 120;
    const int size = 80;
    const int padding = 20;

    int index = 0;

    for (int row = 0; row < SAVE_ROWS; row++) {
        for (int col = 0; col < SAVE_COLS; col++)
        {
            saveSlots[index].bounds = (Rectangle){
                startX + col * (size + padding),
                startY + row * (size + padding),
                size,
                size
            };

            // Label: Slot 1, Slot 2, etc
            static char labels[SAVE_SLOT_COUNT][16];
            sprintf(labels[index], "Slot %d", index + 1);
            saveSlots[index].text = labels[index];

            index++;
        }
	}
}

// Sets the current menu screen and manages cursor visibility
void SetCurrentScreen(MenuScreen newScreen) {
    currentScreen = newScreen;
    static MenuScreen lastScreen = menu_main;

    if (currentScreen != lastScreen)
    {
        if (currentScreen == menu_game || currentScreen == menu_editor) DisableCursor();
        else EnableCursor();
        if (currentScreen == menu_editor)
        {
            ResetProps();
        }
        lastScreen = currentScreen;
    }
}

// Sets whether the UI is currently being interacted with
void SetUIInteraction(bool interaction)
{
	uiInteraction = interaction;
	printf("UI Interaction set to: %s\n", interaction ? "true" : "false");
}

bool IsExitRequested(void) {
    return gExitRequested;
}

void RequestExit(void) {
    gExitRequested = true;
}