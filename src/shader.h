#pragma once
#ifndef VISION_SHADER_H
#define VISION_SHADER_H

#include "includes.h"

void InitVisionShader(int screenWidth, int screenHeight);
void DestroyVisionShader();
void BeginVisionRender();
void EndVisionRender();
void UpdateVisionShader(float deltaTime);

//todo: attribute agnostic shader loading system.. current values are baked in

#endif
