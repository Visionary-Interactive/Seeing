#pragma once
#ifndef MAP_H
#define MAP_H

#include "includes.h"

#define MAX_MAP_X 100
#define MAX_MAP_Y 40
#define MAX_MAP_Z 100

#define MAX_CUBES MAX_MAP_X * MAX_MAP_Y * MAX_MAP_Z

typedef struct Map {
    //Block (*blocks)[MAX_MAP_Y][MAX_MAP_Z];
    Matrix *transforms;
    Mesh cubeMesh; //should be pulling textures from the object pool eventually
    Material matInstances;
    Material matDefault;
    int blockCount;
} Map;

static const int diffx[] = { -1, 1, 0, 0, 0, 0 };
static const int diffy[] = {  0, 0,-1, 1, 0, 0 };
static const int diffz[] = {  0, 0, 0, 0,-1, 1 };

//something like this...
bool InitMap(Map *map, const char *mapPath);
void DrawMap(const Map *map);
void SaveMap(Map *map, const char *mapPath);
bool LoadMap(Map *map, const char *filename);
void UnloadMap(Map *map);
void BuildTransforms(Map *map);

#endif
