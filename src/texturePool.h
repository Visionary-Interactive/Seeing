#ifndef TEXTURE_POOL_H
#define TEXTURE_POOL_H

#include "includes.h"

#define MAX_CACHED_TEXTURES 64

typedef struct {
    char* path;
    Texture2D texture;
} CachedTexture;

typedef struct {
    CachedTexture entries[MAX_CACHED_TEXTURES];
    int count;
} TextureCache;

void InitTextureCache(void);
Texture2D* GetCachedTexture(const char* path);
void UnloadTextureCache(void);

#endif