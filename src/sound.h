#pragma once
#ifndef SOUND_H
#define SOUND_H

#include "includes.h"

extern Sound btnClick;
extern Sound pickupItem1;
extern Sound placeItem1;
extern Sound menuOpen;
extern Sound menuClose;
extern Sound menuMusic;
extern Sound level1Music;

void InitSoundSystem();
void DestroySoundSystem();

#endif
