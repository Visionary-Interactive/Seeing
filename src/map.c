#include "map.h"

static Model pillar;

bool InitMap(Map *map, const char *mapPath)
{
    pillar = LoadModel("resources/global/models/pillar/scene.gltf");
    Texture2D texture = LoadTexture("resources/global/models/pillar/textures/Material_baseColor.png");
    pillar.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    return true;
}

void LoadPropTest(Props* props)
{
    CreateProp(props, pillar, (Vector3){100.0, 0.0, 100.0}, (Vector3) {0.1, 0.1, 0.1}, GRAY, PROP_VISIBILE | PROP_COLLIDER);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, 
                (Vector3) {5.0f - 2.0f * j, 10.0f - 2.0f * i, 10.0f}, 
                (Vector3) {1.0, 1.0, 1.0}, GREEN, PROP_VISIBILE | PROP_COLLIDER);
        }
    }
    /*int puzzle = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) {5.0f, 8.0f, 10.0f}, (Vector3) {1.0, 1.0, 1.0}, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    
    puzzle = CreateProp(props,(Vector3) {3.0f, 10.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), GREEN, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {1.0f, 10.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {-1.0f, 10.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props, (Vector3) {-3.0f, 10.0f, 10.0f}, (Vector3) {1.0, 1.0, 1.0}, LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);

    puzzle = CreateProp(props, (Vector3) {5.0f, 8.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), PURPLE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {3.0f, 8.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), YELLOW, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {1.0f, 8.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), RED, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {-1.0f, 8.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), PURPLE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props, (Vector3) {-3.0f, 8.0f, 10.0f}, (Vector3) {1.0, 1.0, 1.0}, LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);

    puzzle = CreateProp(props, (Vector3) {5.0f, 6.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {3.0f, 6.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), GREEN, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {1.0f, 6.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), GREEN, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {-1.0f, 6.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props, (Vector3) {-3.0f, 6.0f, 10.0f}, (Vector3) {1.0, 1.0, 1.0}, LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);

    puzzle = CreateProp(props, (Vector3) {5.0f, 4.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), RED, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {3.0f, 4.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), RED, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {1.0f, 4.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), PURPLE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {-1.0f, 4.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), YELLOW, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props, (Vector3) {-3.0f, 4.0f, 10.0f}, (Vector3) {1.0, 1.0, 1.0}, LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), GREEN, PROP_VISIBILE | PROP_COLLIDER);

    puzzle = CreateProp(props, (Vector3) {5.0f, 2.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {3.0f, 2.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), GREEN, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {1.0f, 2.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props,(Vector3) {-1.0f, 2.0f, 10.0f},(Vector3) {1.0, 1.0, 1.0},LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), GREEN, PROP_VISIBILE | PROP_COLLIDER);
    puzzle = CreateProp(props, (Vector3) {-3.0f, 2.0f, 10.0f}, (Vector3) {1.0, 1.0, 1.0}, LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER);
*/
	//Create the door PROP to interact with

    CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, 
        (Vector3) {-4.0f, 2.0f, -10.0f}, 
        (Vector3) {1.0, 1.0, 1.0}, GREEN, PROP_VISIBILE | PROP_COLLIDER);

	int doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) {-4.0f, 2.0f, -10.0f},
		(Vector3) {1.0f, 1.0f, 1.0f}, GREEN, PROP_VISIBILE | PROP_COLLIDER);

    doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) {0.0f, 2.0f, -10.0f},
    (Vector3) {1.0f, 1.0f, 1.0f}, GREEN, PROP_VISIBILE | PROP_COLLIDER);

    doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) {4.0f, 2.0f, -10.0f},
    (Vector3) {1.0f, 1.0f, 1.0f}, GREEN, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_DOOR); //holy hell change this
    props->interactType[doorID] = INTERACTABLE_DOOR; //this sucks

    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM, 
        (Vector3) {-2.0f, 1.0f, -5.0f}, 
		(Vector3) {
		1.0f, 1.0f, 1.0f
	}, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        0.0f, -1.0f, 0.0f
    },
        (Vector3) {
        1.0f, 1.0f, 1.0f
    }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);


    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        5.0f, 4.0f, 3.0f
    },
        (Vector3) {
        1.0f, 1.0f, 1.0f
    }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

	int pickupID = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, 
        (Vector3) {-10, 4.0, 0}, (Vector3) {1.0, 1.0, 1.0}, 
        BLUE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PICKUP);
	props->interactType[pickupID] = INTERACTABLE_PICKUP; //this also sucks

    Vector3 lensPos  = (Vector3){ 3.25f, 2.0f, -2.1f };
    Vector3 lensSize = (Vector3){ 2.0f, 4.0f, 2.0f };
    CreateLensProp(props, lensPos, lensSize);
}

void DrawFloor()//const Map *map)
{
    //creates a basic floor grid that we can multiply
    const int floorExtent = 25;
    const float tileSize = 5.0f;
    const Color tileColor1 = (Color){ 150, 200, 200, 255 };

	// Floor tiles to create a checkerboard pattern
    for (int y = -floorExtent; y < floorExtent; y++)
    {
        for (int x = -floorExtent; x < floorExtent; x++)
        {
            if ((y & 1) && (x & 1)) DrawPlane((Vector3) { x* tileSize, 0.0f, y* tileSize }, (Vector2) { tileSize, tileSize }, tileColor1);
            else if (!(y & 1) && !(x & 1)) DrawPlane((Vector3) { x* tileSize, 0.0f, y* tileSize }, (Vector2) { tileSize, tileSize }, PURPLE);
        }
    }

	// Towers at corners
    const Vector3 towerSize = (Vector3){ 16.0f, 32.0f, 16.0f };
    const Color towerColor = (Color){ 150, 200, 200, 255 };

	//premade draws within the raylib library to draw cubes
    Vector3 towerPos = (Vector3){ 32.0f, 16.0f, 32.0f };
    DrawCubeV(towerPos, towerSize, towerColor);
    DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

    towerPos.x *= -1;
    DrawCubeV(towerPos, towerSize, towerColor);
    DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

    towerPos.z *= -1;
    DrawCubeV(towerPos, towerSize, towerColor);
    DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

    towerPos.x *= -1;
    DrawCubeV(towerPos, towerSize, towerColor);
    DrawCubeWiresV(towerPos, towerSize, DARKBLUE);
}

void SaveMapFile(Map *map, const char *mapPath)
{
    FILE *f = fopen(TextFormat("%s/map.bin", mapPath), "wb");
    if (!f)
    {
        TraceLog(LOG_ERROR, "Failed to open map file for writing!");
        return;
    }

    uint8_t version[2] = { MAJOR_VERSION, MINOR_VERSION };
    fwrite(version, sizeof(uint8_t), 2, f);

    Props *props = GetPropStructure();
    uint32_t count = (uint32_t)props->count;
    fwrite(&count, sizeof(uint32_t), 1, f);

    for (uint32_t i = 0; i < count; ++i)
    {
        PropRecord rec = {
            .prim = props->prim[i],
            .position = props->position[i],
            .size = props->size[i],
            .color = props->color[i],
            .components = props->components[i],
            .interactRange = props->interactRange[i],
            .lightColor = props->lightColor[i],
            .lightIntensity= props->lightIntensity[i],
            .interactType = props->interactType[i],
            .scriptID = props->scriptID[i]
        };

        fwrite(&rec, sizeof(PropRecord), 1, f);
    }

    //model path todo
    /*uint16_t len = (uint16_t)strlen(props->modelPath[i]);
    fwrite(&len, sizeof(uint16_t), 1, f);
    fwrite(props->modelPath[i], sizeof(char), len, f);*/

    fclose(f);
    TraceLog(LOG_INFO, "Map saved successfully.");
}

void SaveMapProgress(Map *map, Player *player, const char *mapPath)
{
    FILE *f = fopen(TextFormat("%s/map_mod1.bin", mapPath), "wb");
    if (!f)
    {
        TraceLog(LOG_ERROR, "Failed to open map file for writing!");
        return;
    }

    uint8_t version[2] = { MAJOR_VERSION, MINOR_VERSION };
    fwrite(version, sizeof(uint8_t), 2, f);

    Props *props = GetPropStructure();
    uint32_t count = (uint32_t)props->count;
    fwrite(&count, sizeof(uint32_t), 1, f);

    for (uint32_t i = 0; i < count; ++i)
    {
        PropRecord rec = {
            .prim = props->prim[i],
            .position = props->position[i],
            .size = props->size[i],
            .color = props->color[i],
            .components = props->components[i],
            .interactRange = props->interactRange[i],
            .lightColor = props->lightColor[i],
            .lightIntensity= props->lightIntensity[i],
            .interactType = props->interactType[i],
            .scriptID = props->scriptID[i]
        };

        fwrite(&rec, sizeof(PropRecord), 1, f);
    }

    PlayerRecord rec = {
        .position = player->position,
        .velocity = player->velocity,
        .yaw = player->yaw,
        .pitch = player->pitch,
        .speed = player->speed,
        .isGrounded = player->isGrounded ? 1 : 0
    };
    fwrite(&rec, sizeof(PlayerRecord), 1, f);

    fclose(f);
    TraceLog(LOG_INFO, "Map progress saved successfully.");
}

static int RebuildPropFromRecord(Props* props, const PropRecord* rec)
{
    if (rec->prim == PRIMITIVE_MODEL_LENS)
    {
        return CreateLensProp(props, rec->position, rec->size);
    }

    if (rec->prim != NO_PRIM)
    {
        return CreatePropPrimitive(props, rec->prim, rec->position, rec->size, rec->color, rec->components);
    }

    //non-primitive placeholder
    Model fallback = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    return CreateProp(props, fallback, rec->position, rec->size,
                      rec->color, rec->components);
}

void LoadMapFile(Map *map, const char *mapPath)
{
    (void)map;

    FILE *f = fopen(TextFormat("%s/map.bin", mapPath), "rb");
    if (!f)
    {
        TraceLog(LOG_ERROR, "Failed to open map file for loading!");
        return;
    }

    uint8_t version[2];
    fread(version, sizeof(uint8_t), 2, f);

    if (version[0] < MAJOR_VERSION || (version[0] == MAJOR_VERSION && version[1] < MINOR_VERSION))
    {
        TraceLog(LOG_WARNING, "Map version (%u,%u) is older than expected (%u,%u); some elements may not work.",
                 version[0], version[1], MAJOR_VERSION, MINOR_VERSION);
    }

    Props *props = GetPropStructure();
    memset(props, 0, sizeof(Props));

    uint32_t count = 0;
    fread(&count, sizeof(uint32_t), 1, f);

    for (uint32_t i = 0; i < count; ++i)
    {
        PropRecord rec;
        fread(&rec, sizeof(PropRecord), 1, f);

        int id = RebuildPropFromRecord(props, &rec);
        if (id < 0) continue;

        props->interactRange[id]  = rec.interactRange;
        props->lightColor[id]     = rec.lightColor;
        props->lightIntensity[id] = rec.lightIntensity;
        props->components[id]     = rec.components;
        props->interactType[id]   = rec.interactType;
        props->scriptID[id]       = rec.scriptID;
    }

    fclose(f);
    TraceLog(LOG_INFO, "Map loaded successfully.");
}

void LoadMapProgress(Map *map, Player *player, const char *mapPath)
{
    (void)map;

    FILE *f = fopen(TextFormat("%s/map_mod1.bin", mapPath), "rb");
    if (!f)
    {
        TraceLog(LOG_ERROR, "Failed to open map file for loading!");
        return;
    }

    uint8_t version[2];
    fread(version, sizeof(uint8_t), 2, f);

    if (version[0] < MAJOR_VERSION || (version[0] == MAJOR_VERSION && version[1] < MINOR_VERSION))
    {
        TraceLog(LOG_WARNING, "Map version (%u,%u) is older than expected (%u,%u); some elements may not work.",
                 version[0], version[1], MAJOR_VERSION, MINOR_VERSION);
    }

    Props *props = GetPropStructure();
    ResetProps();

    uint32_t count = 0;
    fread(&count, sizeof(uint32_t), 1, f);

    for (uint32_t i = 0; i < count; ++i)
    {
        PropRecord rec;
        fread(&rec, sizeof(PropRecord), 1, f);

        int id = RebuildPropFromRecord(props, &rec);
        if (id < 0) continue;

        props->interactRange[id]  = rec.interactRange;
        props->lightColor[id]     = rec.lightColor;
        props->lightIntensity[id] = rec.lightIntensity;
        props->components[id]     = rec.components;
        props->interactType[id]   = rec.interactType;
        props->scriptID[id]       = rec.scriptID;
    }

    PlayerRecord rec;
    if (fread(&rec, sizeof(PlayerRecord), 1, f) != 1)
    {
        TraceLog(LOG_ERROR, "The loaded player record is hosed.");
        fclose(f);
        return;
    }

    player->position = rec.position;
    player->velocity = rec.velocity;
    player->yaw = rec.yaw;
    player->pitch = rec.pitch;
    player->speed = rec.speed;
    player->isGrounded = rec.isGrounded != 0;
    player->bottom = player->position.y - player->size.y;

    fclose(f);
    TraceLog(LOG_INFO, "Map checkpoint loaded successfully.");
}