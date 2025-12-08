#pragma once
#ifndef MENU_H
#define MENU_H

#include "includes.h"

static bool uiInteraction;

typedef enum MenuScreen {
	menu_main,
	menu_game,
	menu_game_paused,
	menu_options,
	menu_level_select,
	menu_multi,
	menu_save

} MenuScreen;

typedef struct Button
{
	Rectangle bounds;
	const char* text;

} Button;

void DrawMenu();
void DrawUI();
void DrawButton(struct Button button, Color color);
void DrawTextBox(Rectangle bounds, char* buffer, int currentSize, int maxSize, bool* focused);

MenuScreen GetCurrentScreen();
void SetCurrentScreen(MenuScreen newScreen);
void SetUIInteraction(bool interaction);

bool IsExitRequested(void);
void RequestExit(void);

#endif // !MENU_H

