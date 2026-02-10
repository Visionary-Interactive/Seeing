#pragma once

#include "raylib.h"
#include "floor.h"
#include "raymath.h"
#include "rlgl.h"

typedef struct {
    Model model;
    Texture2D texture;
} Floor;

bool InitFloor(const char* texturePath, float width, float length, float tileRepeat);
Floor* GetFloor(void);
void DrawFloor(Vector3 position, Color tint);
void UnloadFloor(void);