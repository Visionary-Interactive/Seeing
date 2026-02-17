#pragma once
#ifndef MAP_H
#define MAP_H

#include "includes.h"
#include "prop.h"
#include "player.h"
#include "lens.h"
#include "floor.h"

#define MAX_MAP_X 100
#define MAX_MAP_Y 40
#define MAX_MAP_Z 100

#define MAX_CUBES MAX_MAP_X * MAX_MAP_Y * MAX_MAP_Z

// The idea is that the map is contructed out of voxels for the terrain, and props for everything else (lenses, mirrors, lights, items).
// As far as what those two structs looks like we still need to figure that out. But the overall structure of the map is something like this.

typedef struct Map {
    Matrix *transforms;
    Mesh cubeMesh; //should be pulling textures from the object pool eventually
    Material matInstances;
    Material matDefault;
    int blockCount;
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
} PropRecord;


//something like this...
bool InitMap(Map *map, const char *mapPath);
void DrawMap(void);//const Map *map);
//void DrawFloor(void);

void SaveMapFile(Map *map, const char *mapPath);
void SaveMapProgress(Map *map, Player *player, const char *mapPath);
void LoadPropTest(Props* props);
void LoadMapFile(Map *map, const char *mapPath);
void LoadMapProgress(Map *map, Player *player, const char *mapPath);
void UnloadMap(Map *map);
void BuildTransforms(Map *map);

#endif
