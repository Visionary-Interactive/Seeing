#include "modelPool.h"
#include "prop.h"

static ModelCache cache = { 0 };

void InitModelCache(void)
{
    cache.count = 0;
    memset(cache.entries, 0, sizeof(cache.entries));
}

Model* GetCachedModel(const char* path)
{
    // Check if already loaded
    for (int i = 0; i < cache.count; i++)
    {
        if (cache.entries[i].path && strcmp(cache.entries[i].path, path) == 0)
        {
            return &cache.entries[i].model;
        }
    }

    // Not found — load it
    if (cache.count >= MAX_CACHED_MODELS)
    {
        printf("ERROR: Model cache full! Cannot load: %s\n", path);
        return NULL;
    }

    printf("ModelCache: Loading new model '%s' (slot %d)\n", path, cache.count);

    CachedModel* entry = &cache.entries[cache.count];
    entry->path = strdup(path);
    entry->model = LoadModel(path);
    cache.count++;

    return &entry->model;
}

Model* GetCachedPrimitive(PrimitiveModelId prim)
{
    const char* key = NULL;

    switch (prim)
    {
        case PRIMITIVE_MODEL_CUBE: key = "__cube"; break;
        case PRIMITIVE_MODEL_DOOR: key = "__door"; break;
        case PRIMITIVE_MODEL_LENS: key = "__lens"; break;
        case PRIMITIVE_MODEL_PLATFORM: key = "__platform"; break;
        case PRIMITIVE_MODEL_WALL: key = "__wall"; break;
        case PRIMITIVE_MODEL_BUTTON: key = "__button"; break;
        default: return NULL;
    }

    //check cache first (same as GetCachedModel)
    for (int i = 0; i < cache.count; i++)
    {
        if (cache.entries[i].path && strcmp(cache.entries[i].path, key) == 0)
        {
            return &cache.entries[i].model;
        }
    }

    //not found, create it
    if (cache.count >= MAX_CACHED_MODELS)
    {
        printf("ERROR: Model cache full (primitive %s)\n", key);
        return NULL;
    }

    CachedModel* entry = &cache.entries[cache.count];
    entry->path = strdup(key);

    printf("ModelCache: Creating primitive '%s' (slot %d)\n", key, cache.count);

    // Generate mesh based on primitive
    switch (prim)
    {
        case PRIMITIVE_MODEL_CUBE:
            entry->model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
            break;

        case PRIMITIVE_MODEL_DOOR:
            entry->model = LoadModelFromMesh(GenMeshCube(3.0f, 4.0f, 0.5f));
            break;

        case PRIMITIVE_MODEL_LENS:
            entry->model = LoadModelFromMesh(GenMeshSphere(2.0f, 32, 32));
            break;

        case PRIMITIVE_MODEL_PLATFORM:
            entry->model = LoadModelFromMesh(GenMeshCube(4.0f, 0.5f, 4.0f));
            break;

        case PRIMITIVE_MODEL_WALL:
            entry->model = LoadModelFromMesh(GenMeshCube(0.5f, 4.0f, 4.0f));
            break;

        case PRIMITIVE_MODEL_BUTTON:
            entry->model = LoadModelFromMesh(GenMeshCube(1.0f, 0.1f, 1.0f));
            break;

        default:
            return NULL;
    }

    cache.count++;
    return &entry->model;
}

void UnloadModelCache(void)
{
    for (int i = 0; i < cache.count; i++)
    {
        if (cache.entries[i].path)
        {
            UnloadModel(cache.entries[i].model);
            free(cache.entries[i].path);
            cache.entries[i].path = NULL;
        }
    }
    cache.count = 0;
    printf("ModelCache: All models unloaded.\n");
}