#pragma once
#ifndef MENU_H
#define MENU_H

#include "includes.h"

#include "player.h"

static bool uiInteraction;

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

typedef struct TextBox
{
	bool active;
	Rectangle bounds;
	const char* text;
} TextBox;

TextBox characterBox;

void DrawMenu();
void DrawUI(struct InventoryItem item[INVENTORY_SIZE], int selectedSlot);
void DrawButton(struct Button button, Color color);
void DrawTextBox(Rectangle bounds, char* buffer, int currentSize, int maxSize, bool* focused);

void DrawInteractTextBox();

void UpdateInteractTextBox();

MenuScreen GetCurrentScreen();

void InitSaveSlots();
void SetCurrentScreen(MenuScreen newScreen);
void SetUIInteraction(bool interaction);

bool IsExitRequested(void);
void RequestExit(void);

#endif // !MENU_H

