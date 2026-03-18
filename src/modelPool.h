#ifndef MODEL_POOL_H
#define MODEL_POOL_H

#include "includes.h"

#define MAX_CACHED_MODELS 64

typedef enum PrimitiveModelId PrimitiveModelId;

typedef struct {
    char* path;
    Model model;
} CachedModel;

typedef struct {
    CachedModel entries[MAX_CACHED_MODELS];
    int count;
} ModelCache;

void InitModelCache(void);
Model* GetCachedModel(const char* path);
Model* GetCachedPrimitive(PrimitiveModelId prim);
void UnloadModelCache(void);

#endif