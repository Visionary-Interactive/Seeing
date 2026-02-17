#include "sound.h"
#include "raudio.c"

Sound btnClick;
Sound pickupItem1;
Sound menuMusic;
Sound level1Music;

void InitSoundSystem()
{
	InitAudioDevice();

	btnClick = LoadSound("resources/sound/sfx/button.wav");
	pickupItem1 = LoadSound("resources/sound/sfx/pickupItem1.wav");
	menuMusic = LoadSound("resources/sound/bgm/MenuBGM.wav");
	level1Music = LoadSound("resources/sound/bgm/Level1BGM.wav");
	menuMusic.stream.buffer->looping = true;
	level1Music.stream.buffer->looping = true;
}

void DestroySoundSystem()
{
	UnloadSound(btnClick);
	UnloadSound(pickupItem1);
	UnloadSound(menuMusic);
	UnloadSound(level1Music);
	CloseAudioDevice();
}