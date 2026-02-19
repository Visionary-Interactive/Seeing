#pragma once
#ifndef MENU_H
#define MENU_H

#include "includes.h"
#include "player.h"

static bool uiInteraction;

typedef struct InventoryItem InventoryItem;

typedef enum MenuScreen {
	menu_main,
	menu_game,
	menu_game_paused,
	menu_options,
	menu_editor,
	menu_level_select,
	menu_multi,
	menu_multi2,
	menu_save

} MenuScreen;

typedef struct Button
{
	Rectangle bounds;
	const char* text;

} Button;

typedef enum {
	TEXTBOX_NONE,
	TEXTBOX_PLAYER,
	TEXTBOX_BOOK
} TextboxType;

typedef struct TextBox
{
	TextboxType type;
	bool active;
	Rectangle bounds;
	char text[512];
	float timer;
	float duration;
} TextBox;


void DrawMenu();
void DrawUI(InventoryItem *item, int selectedSlot);
void DrawButton(struct Button button, Color color);
void DrawTextBox(Rectangle bounds, char* buffer, int currentSize, int maxSize, bool* focused);

void InitTextBox(TextboxType type, const char* text);

void UpdateTextBox(float deltaTime);

void DrawCharacterbox();

void DrawInteractTextBox();

MenuScreen GetCurrentScreen();

void InitSaveSlots();
void SetCurrentScreen(MenuScreen newScreen);
void SetUIInteraction(bool interaction);

bool IsExitRequested(void);

bool IsTextboxStoppingPlayer();
void RequestExit(void);

#endif