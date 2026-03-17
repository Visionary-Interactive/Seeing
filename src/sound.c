#include "sound.h"
#include "raudio.c"

Sound btnClick;
Sound pickupItem1;
Sound placeItem1;
Sound menuOpen;
Sound menuClose;
Sound menuMusic;
Sound level1Music;
Sound rotatingPuzzleBlock;

void InitSoundSystem()
{
	InitAudioDevice();

	btnClick = LoadSound("resources/sound/sfx/button2.wav");
	pickupItem1 = LoadSound("resources/sound/sfx/pickupItem1.wav");
	placeItem1 = LoadSound("resources/sound/sfx/placeItem1.wav");
	menuOpen = LoadSound("resources/sound/sfx/MenuOpen.wav");
	menuClose = LoadSound("resources/sound/sfx/MenuClose.wav");
	menuMusic = LoadSound("resources/sound/bgm/MenuBGM.wav");
	level1Music = LoadSound("resources/sound/bgm/Level1BGM.wav");
	rotatingPuzzleBlock = LoadSound("resources/sound/sfx/rotating_block.wav");
	menuMusic.stream.buffer->looping = true;
	level1Music.stream.buffer->looping = true;
}

void DestroySoundSystem()
{
	UnloadSound(btnClick);
	UnloadSound(pickupItem1);
	UnloadSound(placeItem1);
	UnloadSound(menuOpen);
	UnloadSound(menuClose);
	UnloadSound(menuMusic);
	UnloadSound(level1Music);
	UnloadSound(rotatingPuzzleBlock);
	CloseAudioDevice();
}