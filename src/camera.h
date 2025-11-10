#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "includes.h"
#include "player.h"

extern Camera* cam;
extern int cameraMode;

void InitCamera();
Camera* GetCamera();
void RefreshCamera(Player* player);
void DestroyCamera();

#endif
