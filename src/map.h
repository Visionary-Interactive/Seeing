#pragma once
#ifndef MAP_H
#define MAP_H

#include "includes.h"
#include "prop.h"
#include "player.h"
#include "lens.h"
#include "floor.h"
#include "sessionStateController.h"

#define MAX_MAP_X 100
#define MAX_MAP_Y 40
#define MAX_MAP_Z 100

#define MAX_CUBES MAX_MAP_X * MAX_MAP_Y * MAX_MAP_Z

typedef struct Map {
	char temp; // Placeholder member to allow an empty struct definition
} Map;

typedef struct PlayerRecord {
    Vector3 position;
    Vector3 velocity;
    float pitch;
    float yaw;
    float speed;
    bool isGrounded;
} PlayerRecord;

typedef struct PropRecord {
    PrimitiveModelId prim;
    Vector3 position;
    Vector3 size;
    Color color;
    uint32_t components;
    Vector3 interactRange;
    Color lightColor;
    float lightIntensity;
    int interactType;
    int scriptID;
    char modelPath[PROP_MODEL_PATH_MAX];
    char texturePath[PROP_MODEL_PATH_MAX];
} PropRecord;

//static int PillarID;

bool InitMap(Map *map, const char *mapPath);
void SaveMapFile(Map *map, const char *mapPath);
void SaveMapProgress(Map *map, Player *player, const char *mapPath);

//Level 1
void LoadPropTest();
//Level 2
void LoadLevel2();

void (*currentLevelLoaded)();

void LoadLevel4();

void GenerateCubePuzzle(Props* props, int count,
    float minX, float maxX,
    float minZ, float maxZ,
    Vector3 size, Color color, uint32_t components);

void ResetLevel();

void RetryLevel();

void LoadMapFile(Map *map, const char *mapPath);
void LoadMapProgress(Map *map, Player *player, const char *mapPath);

#endif
