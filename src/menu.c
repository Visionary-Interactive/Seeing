#include "includes.h"
#include "sessionManager.h"
#include "sessionStateController.h"
#include "sessionLobbyController.h"
#include "player.h"
#include "sound.h"
#include "menu.h"

// Define buttons for various menu options
Button playButton = { { 100, 100, 200, 50 }, "Play" };
Button saveButton = { { 100, 200, 200, 50 }, "Save/Load" };
Button levelButton = { { 100, 300, 200, 50 }, "Level Select" };
Button compendiumButton = { { 100, 400, 200, 50 }, "Compendium" };
Button multiMenuButton = { { 100, 600, 200, 50 }, "Multiplayer" };
Button optionsButton = { { 100, 500, 200, 50 }, "Options" };
Button exitButton = { { 100, 700, 200, 50 }, "Exit Game" };
Button menuButton = { { 100, 600, 200, 50 }, "Back to Menu" };
Button multiConnectButton = { { 100, 200, 200, 50 }, "Connect" };
Button level1Button = { { 100, 100, 200, 50 }, "Level 1" };
Button retryButton = { { 100, 100, 200, 50 }, "Retry" };

#define SAVE_COLS 4
#define SAVE_ROWS 3
#define SAVE_SLOT_COUNT (SAVE_COLS * SAVE_ROWS)

#define HOVER_ANIM_SPEED 0.15f

Button saveSlots[SAVE_SLOT_COUNT];

Rectangle ipAddressText = { 400, 200, 200, 50 };
char ipAddress[32] = HOME_SERVER_IP;
bool ipBoxFocused = false;

static MenuScreen currentScreen = menu_main;
static MenuScreen lastScreen;
static bool gExitRequested = false;
static TextBox currentTextbox = { 0 };
static Texture2D flammarion;
static Font greatVibes;
static Font romanica;
static Texture2D buttonTex;
static Rectangle buttonSrcNormal;
static Rectangle buttonSrcHover;

static Texture2D hoverAnimTex;
static Rectangle hoverFrames[3];
static int hoverFrameCount = 3;

static float hoverAnimTimer = 0.0f;
static int hoverFrameIndex = 0;

static void CenterButton(Button* button, float y)
{
    float screenWidth = (float)GetScreenWidth();

    button->bounds.x = screenWidth * 0.80f - button->bounds.width * 0.5f;
    button->bounds.y = y;
}

void LoadMenuElements()
{
    greatVibes = LoadFontEx("resources/global/fonts/greatvibes/GreatVibes-Regular.ttf", 128, NULL, 0);
    romanica = LoadFontEx("resources/global/fonts/romanica/Romanica.ttf", 128, NULL, 0);
    flammarion = LoadTexture("resources/global/tex/flammarion.png");
    buttonTex = LoadTexture("resources/global/tex/seeingButton.png");
    hoverAnimTex = LoadTexture("resources/global/tex/seeingButtonSketch.png");

    int frameWidth = hoverAnimTex.width;
    int frameHeight = hoverAnimTex.height / hoverFrameCount;

    for (int i = 0; i < hoverFrameCount; i++)
    {
        hoverFrames[i] = (Rectangle){
            0,
            i * frameHeight,
            frameWidth,
            frameHeight
        };
    }

    buttonSrcNormal = (Rectangle){ 0, 0, buttonTex.width, buttonTex.height / 2 };
    buttonSrcHover  = (Rectangle){ 0, buttonTex.height / 2, buttonTex.width, buttonTex.height / 2 };

    SetTextureFilter(buttonTex, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(greatVibes.texture, TEXTURE_FILTER_BILINEAR);
}

// Draws the current menu screen based on the current state
void DrawMenu()
{
    MenuScreen screen = GetCurrentScreen();

    switch (screen)
    {
    case menu_main:
    {
        float screenW = GetScreenWidth();
        float screenH = GetScreenHeight();

        float buttonHeight = 100;
        float spacing = 20;
        float totalHeight = (6 * buttonHeight) + (5 * spacing);

        float startY = screenH * 0.5f - totalHeight * 0.5f;

        DrawTextureEx(flammarion, (Vector2){150, GetScreenHeight() * 0.2f},
            0.0f, 0.4f, WHITE
        );

        DrawTextEx(greatVibes, "The Delian Problem", (Vector2){screenW * 0.125f, screenH * 0.05f}, 128.0f, 0.0f, BLACK);

        CenterButton(&levelButton, startY + 0 * (buttonHeight + spacing));
        CenterButton(&saveButton, startY + 1 * (buttonHeight + spacing));
        CenterButton(&compendiumButton, startY + 2 * (buttonHeight + spacing));
        CenterButton(&multiMenuButton, startY + 3 * (buttonHeight + spacing));
        CenterButton(&optionsButton, startY + 4 * (buttonHeight + spacing));
        CenterButton(&exitButton, startY + 5 * (buttonHeight + spacing));

		// Play menu music if not already playing
		if (!IsSoundPlaying(menuMusic)) PlaySound(menuMusic);
		StopSound(level1Music);
        DrawButton(levelButton, DARKGRAY);
        DrawButton(saveButton, DARKGRAY);
        DrawButton(compendiumButton, DARKGRAY);
        DrawButton(multiMenuButton, DARKGRAY);
        DrawButton(optionsButton, DARKGRAY);
        DrawButton(exitButton, DARKGRAY);
        
        
    }
    break;
    case menu_game:
        DrawUI(playerList[0]->inventory, playerList[0]->selectedSlot);
        break;

    case menu_options:
        DrawText("OPTIONS", 100, 40, 30, ORANGE);
        //DrawTextEx(romanican, "OPTIONS", (Vector2){100, 40}, 30, ORANGE);
		DrawButton(menuButton, DARKGRAY);
        break;

    case menu_level_select:
       DrawText("LEVEL SELECT", 100, 40, 30, GREEN);
	   CenterButton(&level1Button, 200);
	   CenterButton(&menuButton, 600);
       DrawButton(menuButton, DARKGRAY);
	   DrawButton(level1Button, DARKGRAY);
		break;

    case menu_editor:
        DrawUI(playerList[0]->inventory, playerList[0]->selectedSlot);
        break;

    case menu_multi:
		DrawText("MULTIPLAYER", 100, 40, 30, BLUE);
		//DrawTextBox(ipAddressText, ipAddress, strlen(ipAddress), 32, &ipBoxFocused);
		DrawButton(multiConnectButton, DARKGRAY);
        DrawButton(menuButton, DARKGRAY);
        break;

    case menu_multi2:
		DrawText("MULTIPLAYER", 100, 40, 30, BLUE);
		//DrawTextBox(ipAddressText, ipAddress, strlen(ipAddress), 32, &ipBoxFocused);
        DrawButton(menuButton, DARKGRAY);
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
    {
        float screenH = GetScreenHeight();

        float buttonHeight = 50;
        float spacing = 20;
        float totalHeight = (5 * buttonHeight) + (4 * spacing);
        float startY = screenH * 0.5f - totalHeight * 0.5f;

        CenterButton(&playButton, startY + 0 * (buttonHeight + spacing));
        CenterButton(&retryButton, startY + 1 * (buttonHeight + spacing));
        CenterButton(&saveButton, startY + 2 * (buttonHeight + spacing));
        CenterButton(&optionsButton, startY + 3 * (buttonHeight + spacing));
        CenterButton(&menuButton, startY + 4 * (buttonHeight + spacing));

        DrawButton(playButton, DARKGRAY);
        DrawButton(retryButton, DARKGRAY);
        DrawButton(saveButton, DARKGRAY);
        DrawButton(optionsButton, DARKGRAY);
        DrawButton(menuButton, DARKGRAY);
    }
    break;


    default:
        DrawText("UNKNOWN SCREEN", 100, 40, 30, RED);
        break;
    }

    //update the button anim
    hoverAnimTimer += GetFrameTime();

    if (hoverAnimTimer >= HOVER_ANIM_SPEED)
    {
        hoverAnimTimer = 0.0f;
        hoverFrameIndex++;

        if (hoverFrameIndex >= hoverFrameCount)
            hoverFrameIndex = 0;
    }
}

//Draws a button and handles its interaction logic
void DrawButton(Button button, Color color)
{
    bool hovered = CheckCollisionPointRec(GetMousePosition(), button.bounds);

    Rectangle src = hovered ? buttonSrcHover : buttonSrcNormal;

    DrawTexturePro(
        buttonTex,
        src,
        button.bounds,
        (Vector2){0,0},
        0.0f,
        WHITE
    );

    float fontSize = 40.0f;
    float spacing = 0.0f;

    Vector2 textSize = MeasureTextEx(romanica, button.text, fontSize, spacing);

    DrawTextEx(
        romanica,
        button.text,
        (Vector2){
            button.bounds.x + button.bounds.width * 0.5f - textSize.x * 0.5f,
            button.bounds.y + button.bounds.height * 0.5f - textSize.y * 0.5f
        },
        fontSize,
        spacing,
        BLACK
    );

    if (hovered)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            PlaySound(btnClick);

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
            {
                SessionManager_Client_Disonnect();
                multiplayerSession = false;
                SetCurrentScreen(menu_main);
            }
            else if (strcmp(button.text, "Multiplayer") == 0)
                SetCurrentScreen(menu_multi);
            else if (strcmp(button.text, "Connect") == 0)
            {
                ConnectToHomeServer();
                SetCurrentScreen(menu_multi2);
            }
            else if (strcmp(button.text, "Retry") == 0)
            {
                ResetPlayerToSpawn(playerList[0]);
                SetCurrentScreen(menu_game);
            }
            else if (strcmp(button.text, "Level Select") == 0)
                SetCurrentScreen(menu_level_select);
            else if (strcmp(button.text, "Save/Load") == 0)
                SetCurrentScreen(menu_save);
        }

        float animationPadding = 20.0f; 
        Rectangle hoverBounds = {
            button.bounds.x - animationPadding,
            button.bounds.y - animationPadding,
            button.bounds.width + 2 * animationPadding,
            button.bounds.height + 2 * animationPadding
        };

        DrawTexturePro(
            hoverAnimTex,
            hoverFrames[hoverFrameIndex],
            hoverBounds,
            (Vector2){0,0},
            0.0f,
            Fade(WHITE, 0.9f)
        );
    }
}

//will draw interaction to the UI for the player
void DrawUI(InventoryItem *item, int selectedSlot)
{
    if (currentTextbox.active)
    {
        DrawCharacterbox();
		printf("Current TextBox Active: %s\n", currentTextbox.text);
    }

    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        Color c = (i == selectedSlot) ? YELLOW : GRAY;
        DrawRectangle(500 + i * 100, 850, 80, 80, c);

        if (item[i].occupied)
            DrawText("X", 540 + i * 100, 860, 20, BLACK);
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

void DrawInteractTextBox()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Size of the book page (80% of screen)
    int boxWidth = screenWidth * 0.8f;
    int boxHeight = screenHeight * 0.8f;

    // Centered position
    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = (screenHeight - boxHeight) / 2;

    // Optional: darken background behind book
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));

    // White page
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, RAYWHITE);

    // Black border
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, BLACK);

    // Text padding inside page
    int padding = 40;

    DrawText(
        currentTextbox.text,
        boxX + padding,
        boxY + padding,
        24,
        BLACK
    );

    // Close hint
    DrawText(
        "",
        boxX + boxWidth - 200,
        boxY + boxHeight - 40,
        18,
        DARKGRAY
    );
}


void InitTextBox(TextboxType type, const char* tText)
{
	printf("Initializing TextBox of type %d with text: %s\n", type, tText);
    currentTextbox.type = type;
    currentTextbox.active = true;
    strncpy(currentTextbox.text, tText, sizeof(currentTextbox.text));

    if (type == TEXTBOX_PLAYER)
    {
        currentTextbox.timer = 0.0f;
        currentTextbox.duration = 4.0f; // lasts 4 seconds
    }
}

void UpdateTextBox(float deltaTime)
{
    if (!currentTextbox.active) return;

    if (currentTextbox.type == TEXTBOX_PLAYER)
    {
        currentTextbox.timer += deltaTime;

        if (currentTextbox.timer >= currentTextbox.duration)
        {
            currentTextbox.active = false;
        }
    }
    else if (currentTextbox.type == TEXTBOX_BOOK)
    {
        if (IsKeyPressed(KEY_R))
        {
            currentTextbox.active = false;
        }
    }
}

void DrawCharacterbox()
{
    if (!currentTextbox.active) return;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (currentTextbox.type == TEXTBOX_PLAYER)
    {
        // Bottom RPG style textbox
        DrawRectangle(
            50,
            screenHeight - 180,
            screenWidth - 100,
            130,
            Fade(BLACK, 0.85f)
        );

        DrawText(
            currentTextbox.text,
            80,
            screenHeight - 150,
            20,
            RAYWHITE
        );
    }
    else if (currentTextbox.type == TEXTBOX_BOOK)
    {
        // Full screen reading textbox
        DrawRectangle(
            50,
            50,
            screenWidth - 100,
            screenHeight - 100,
            Fade(BLACK, 0.95f)
        );

        DrawText(
            currentTextbox.text,
            100,
            120,
            24,
            RAYWHITE
        );

        DrawText(
            "Press R to close the page",
            screenWidth - 250,
            screenHeight - 80,
            18,
            GRAY
        );
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
    lastScreen = menu_main;
	static Vector2 currentMousePos = { 0, 0 };
    currentMousePos = GetMousePosition();

    if (currentScreen != lastScreen)
    {
		// Mouse visibility logic
        if (currentScreen == menu_game || currentScreen == menu_editor)
            DisableCursor();
        else
            EnableCursor();

        // SFX
        if (currentScreen == menu_game || currentScreen == menu_editor)
        {
            StopSound(menuMusic); // Stop menu music when entering game/editor
            if (!IsSoundPlaying(level1Music)) PlaySound(level1Music);
        }
        if (lastScreen == menu_game_paused && (currentScreen == menu_game || currentScreen == menu_editor))
        {
			PlaySound(menuClose);
        }
        if (currentScreen == menu_game_paused)
        {
            PlaySound(menuOpen);
        }

        if (currentScreen == menu_editor)
        {
            ResetProps();
        }

        lastScreen = currentScreen;
    }
    SetMousePosition(currentMousePos.x, currentMousePos.y);
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

bool IsTextboxStoppingPlayer()
{
    if (!currentTextbox.active) return false;

    return (currentTextbox.type == TEXTBOX_BOOK);
}

void RequestExit(void) {
    gExitRequested = true;
}