#pragma once
#ifndef MAP_H
#define MAP_H

#include "includes.h"

#define MAX_MAP_X 100
#define MAX_MAP_Y 40
#define MAX_MAP_Z 100

#define MAX_CUBES MAX_MAP_X * MAX_MAP_Y * MAX_MAP_Z

// The idea is that the map is contructed out of voxels for the terrain, and props for everything else (lenses, mirrors, lights, items).
// As far as what those two structs looks like we still need to figure that out. But the overall structure of the map is something like this.

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
bool LoadMap(Map *map, const char *mapPath);
void UnloadMap(Map *map);
void BuildTransforms(Map *map);

#endif
