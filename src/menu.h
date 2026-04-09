#pragma once
#ifndef MENU_H
#define MENU_H

#include "includes.h"
#include "player.h"
#include "TextboxType.h"


static bool uiInteraction;

typedef struct InventoryItem InventoryItem;

typedef enum MenuScreen {
	menu_main,
	menu_game,
	menu_game_paused,
	menu_options,
	menu_editor,
	menu_level_select,
	menu_level_complete,
	menu_multi,
	menu_multi2,
	menu_save,
	menu_compendium
} MenuScreen;

typedef void (*ButtonAction)(void);

typedef struct Button
{
	Rectangle bounds;
	const char* text;
	ButtonAction action;
	ButtonAction action2;
} Button;


typedef struct TextBox
{
	TextboxType type;
	bool active;
	Rectangle bounds;
	char text[512];
	float timer;
	float duration;
} TextBox;

void LoadMenuElements();
void DrawMenu();
void DrawUI(InventoryItem *item, int selectedSlot);
void DrawButton(struct Button button, Color color);
void InitTextBox(TextboxType type, const char* tText);
void UpdateTextBox(float deltaTime);
void DrawCharacterbox();
MenuScreen GetCurrentScreen();
void InitSaveSlots();
void SetCurrentScreen(MenuScreen newScreen);
void SetUIInteraction(bool interaction);
bool IsExitRequested(void);
bool IsTextboxStoppingPlayer();
void RequestExit(void);

#endif