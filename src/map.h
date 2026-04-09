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

typedef enum Level2Patterns{
    PATTERN_CIRCLE,
	PATTERN_X,
	PATTERN_SQUARE,
    PATTERN_TRIANGLE
} Level2Patterns;


 int currentLevel;
//code for level 2 puzzle needed to be passed to player

//static int PillarID;

bool InitMap(Map *map, const char *mapPath);
void SaveMapFile(Map *map, const char *mapPath);
void SaveMapProgress(Map *map, Player *player, const char *mapPath);

//Level 1
void LoadPropTest();
//Level 2
void LoadLevel2();

//Level 2 PatternTypes for puzzle blocks

bool isCircle(int x, int z);
bool isTriangle(int x, int z);
bool isSquare(int x, int z);
bool isX(int x, int z);


//holds the value of the current level
extern void (*currentLevelLoaded)();
void LoadLevel4();

void GenerateCubePuzzle(Props* props, int gridX, int gridZ,
    float minX, float maxX,
    float minZ, float maxZ,
    Vector3 size, Color color, uint32_t components, Level2Patterns pattern);

void GenerateCubePuzzle2(Props* props, int gridX, int gridZ,
    float minZ, float maxZ,
    float minY, float maxY,
    Vector3 size, Color color, uint32_t components, Level2Patterns pattern);

void GenerateLevel2Code(int *code);

void ResetLevel();

void RetryLevel();

void LoadMapFile(Map *map, const char *mapPath);
void LoadMapProgress(Map *map, Player *player, const char *mapPath);

#endif
