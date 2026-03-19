#include "texturePool.h"

static TextureCache cache = { 0 };

void InitTextureCache(void)
{
    cache.count = 0;
    memset(cache.entries, 0, sizeof(cache.entries));
}

Texture2D* GetCachedTexture(const char* path)
{
    //check cache
    for (int i = 0; i < cache.count; i++)
    {
        if (cache.entries[i].path && strcmp(cache.entries[i].path, path) == 0)
        {
            return &cache.entries[i].texture;
        }
    }

    //load new
    if (cache.count >= MAX_CACHED_TEXTURES)
    {
        printf("ERROR: Texture cache full! %s\n", path);
        return NULL;
    }

    printf("TextureCache: Loading '%s' (slot %d)\n", path, cache.count);

    CachedTexture* entry = &cache.entries[cache.count];
    entry->path = strdup(path);
    entry->texture = LoadTexture(path);

    cache.count++;
    return &entry->texture;
}

void UnloadTextureCache(void)
{
    for (int i = 0; i < cache.count; i++)
    {
        if (cache.entries[i].path)
        {
            UnloadTexture(cache.entries[i].texture);
            free(cache.entries[i].path);
        }
    }

    cache.count = 0;
    printf("TextureCache: All textures unloaded.\n");
}