#pragma once
#ifndef MENU_H
#define MENU_H


#include "includes.h"


typedef enum MenuScreen {
	menu_main,
	menu_game,
	menu_game_paused,
	menu_options,
	menu_level_select,
	menu_multi,

} MenuScreen;


typedef struct Button
{
	Rectangle bounds;
	const char* text;

} Button;



void DrawMenu();

void DrawButton(struct Button button, Color color);

MenuScreen GetCurrentScreen();
void SetCurrentScreen(MenuScreen newScreen);








#endif // !MENU_H

