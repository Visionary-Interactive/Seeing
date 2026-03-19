#include "map.h"

static char* pillar;
static char* book;
static char* wall;
static char* wall2;
static char* chair;
static char* wTexP;
static char* bTexP;
static char* pTexP;
static int puzzle1BlockIDs[3];

bool InitMap(Map *map, const char *mapPath)
{
    // Pillar
    /*pillar = LoadModel("resources/global/models/pillar/scene.gltf");
    Texture2D texture = LoadTexture("resources/global/models/pillar/textures/Material_baseColor.png");

    // Book
    book = LoadModel("resources/global/models/book/scene.gltf");
    Texture2D bookTexture = LoadTexture("resources/global/models/book/textures/01_-_Default_baseColor.png");

    // Misc
	chair = LoadModel("resources/assets/chair.glb");*/

    pillar = "resources/global/models/pillar/scene.gltf";
    wTexP = "resources/global/models/pillar/textures/Material_baseColor.png";

    // Book
    book = "resources/global/models/book/scene.gltf";
    Texture2D bookTexture = LoadTexture("resources/global/models/book/textures/01_-_Default_baseColor.png");
    bTexP = "resources/global/models/book/textures/01_-_Default_baseColor.png";

	chair = "resources/assets/chair.glb";

    wall = "resources/global/models/wall/wall.glb";
    wall2 = "resources/global/models/wall2/wall2.glb";

    //pillar.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	//book.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    return true;
}

void DrawMap(void)
{
}

void LoadPropTest(Props* props)
{
    CreatePropFromPath(props, pillar, wTexP,
        (Vector3){40.0, 0.0, 40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, pillar, wTexP,
        (Vector3){-40.0, 0.0, 40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, pillar, wTexP,
        (Vector3){40.0, 0.0, -40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, pillar, wTexP,
        (Vector3){-40.0, 0.0, -40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);

    /*
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, 
                (Vector3) {5.0f - 2.0f * j, 10.0f - 2.0f * i, 10.0f}, 
                (Vector3) {1.0, 1.0, 1.0}, GREEN, PROP_VISIBILE | PROP_COLLIDER);
        }
    }
    */

	int newWallID = CreatePropFromPath(props, wall2, wTexP, (Vector3) { 0.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	//big room back wall2 leftside
    CreatePropFromPath(props, wall2, wTexP,(Vector3) { -8.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP,(Vector3) { -18.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP,(Vector3) { -28.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	//big room back wall rightside
	CreatePropFromPath(props, wall2, wTexP,(Vector3) { 10.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 20.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 26.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

	int newWallID2 = CreatePropFromPath(props, wall, wTexP, (Vector3) { -6.0f, 0.0f, 44.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    //hallway leftside
	CreatePropFromPath(props, wall, wTexP, (Vector3) { -6.0f, 0.0f, 35.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	CreatePropFromPath(props, wall, wTexP, (Vector3) { -6.0f, 0.0f, 26.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -6.0f, 0.0f, 17.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -6.0f, 0.0f, 7.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

    //hallway rightside
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 4.0f, 0.0f, 44.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 4.0f, 0.0f, 35.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 4.0f, 0.0f, 26.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 4.0f, 0.0f, 17.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 4.0f, 0.0f, 7.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);




	//big room leftside
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -33.5f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -33.5f, 0.0f, -37.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -33.5f, 0.0f, -42.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -33.5f, 0.0f, -47.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -33.5f, 0.0f, -52.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);


    //big room rightside
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 30.5f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 30.5f, 0.0f, -37.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 30.5f, 0.0f, -42.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 30.5f, 0.0f, -47.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 30.5f, 0.0f, -52.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

    //big room pillar
    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
		(Vector3) {
		-30, 0.0, -50.0f
	}, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        -30, 0.0, -35.0f
    }, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);


    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        -19, 0.0, -35.0f
    }, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        -8, 0.0, -35.0f
    }, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        0.5, 3.5f, -35.0f
    },
        (Vector3) {
        2.0f, 1.5f, 1.0f
	}, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        19, 0.0, -35.0f
    }, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        30, 0.0, -35.0f
    }, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);


    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        30, 0.0, -44.0f
    }, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);


    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        30, 0.0, -52.0f
    }, (Vector3) { 0.06, 0.01, 0.06 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);



    int pushCubeID = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) { -19.0f, 2.0f, -45.0f }, (Vector3) { 4.0f, 4.0f, 4.0f }, RED, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);
    int pushCubeID2 = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) { 5, 2.0f, -47.0f }, (Vector3) { 4.0f, 4.0f, 4.0f }, ORANGE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);
    int pushCubeID3 = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) { 12, 2.0f, -55.0f }, (Vector3) { 4.0f, 4.0f, 4.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);

	//starting platform for first room
    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        -27.5, 1.5f, -60.0f
    },
        (Vector3) {
        1.0f, 1.5f, 1.0f
    }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

   //middle platform with book
    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        29.0f, 3.5f, -60.0f
    },
        (Vector3) {
        1.0f, 1.5f, 1.0f
    }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

    int doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) { 29.0f, 5.0f, -60.0f },
        (Vector3) {
        1.0f, 1.0f, 1.0f
    }, GREEN, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_DOOR); //holy hell change this
    props->interactType[doorID] = INTERACTABLE_DOOR; //this sucks


    props->interactType[pushCubeID] = INTERACTABLE_PUSH;
	props->interactType[pushCubeID2] = INTERACTABLE_PUSH;
	props->interactType[pushCubeID3] = INTERACTABLE_PUSH;


    int pickupID = CreatePropFromPath(props, "resources/global/models/book/scene.gltf", "resources/global/models/book/textures/01_-_Default_baseColor.png",
        (Vector3) {
		1.5, 4.5f, -35.0f
    }, (Vector3) { 0.03, 0.03, 0.03 },
            WHITE, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
    props->interactType[pickupID] = INTERACTABLE_TEXT;
	props->textType[pickupID] = TEXTBOX_BOOK;
	props->text[pickupID] = "Where am I, I don't know what's going on! \n My feet still seem to work (WASD) and I can Jump pretty well.(Space) \n I need to Get out of here";

	int zoneID = AddZone(props, (Vector3) { 0.0f, 0.0f, -20.0f }, (Vector3) { 10.0f, 10.0f, 10.0f }, TRIGGER_TEXT);
	props->text[zoneID] = "I have entered a new area, I wonder if there is anything different here?";
	props->textType[zoneID] = TEXTBOX_PLAYER;


	AddKillFlame((Vector3) { -2.0f, 0.0f, 40.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);
    AddKillFlame((Vector3) { 1.0f, 0.0f, 40.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);
    AddKillFlame((Vector3) { 4.0f, 0.0f, 40.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);


    AddKillFlame((Vector3) { -2.0f, 0.0f, 30.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 1.0f, 0.0f, 30.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);
    AddKillFlame((Vector3) { 4.0f, 0.0f, 30.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);

    AddKillFlame((Vector3) { -2.0f, 0.0f, 25.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);
    AddKillFlame((Vector3) { 1.0f, 0.0f, 25.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 4.0f, 0.0f, 25.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);

    AddKillFlame((Vector3) { -2.0f, 0.0f, 18.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 1.0f, 0.0f, 18.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);
    AddKillFlame((Vector3) { 4.0f, 0.0f, 18.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);

    AddKillFlame((Vector3) { -2.0f, 0.0f, 10.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 1.0f, 0.0f, 10.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 4.0f, 0.0f, 10.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);

    AddKillFlame((Vector3) { -2.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 1.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 4.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);


	//AddDeadzone(props, (Vector3) { 0.0f, 0.0f, -20.0f }, (Vector3) { 10.0f, 10.0f, 10.0f });

    // Spawning rotating puzzle blocks for testing
    static char blockLetters[3][2]; // 3 blocks, 1 char + null
    for (int i = 0; i < 3; i++)
    {
        int puzzleBlock1 = CreatePropFromPath(props, 
            "resources/global/models/puzzleBlock/PuzzleBlock1.glb",
            "resources/global/models/puzzleBlock/PuzzleBlock1.glb",
            (Vector3) {
			4.75f, 1.8f, -19.0f + i
        },
            (Vector3) {
			0.5f, 0.5f, 0.5f
		},
			WHITE, PROP_VISIBILE | PROP_INTERACTABLE);
		props->interactType[puzzleBlock1] = INTERACTABLE_PUZZLE_ROTATATION_BLOCK;
		//pre-compiler
        blockLetters[i][0] = (char)('0' + i);
		blockLetters[i][1] = '\0';
		props->text[puzzleBlock1] = blockLetters[i]; // Set this so we can tell where the block is facing.
		props->rotation[puzzleBlock1].y = 0.0f + (90.0f * i);
		puzzle1BlockIDs[i] = puzzleBlock1;
	}

	// Pickup blocks for testing
	int testCube = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE,
		(Vector3) {
		4.75f, 1.8f, -10.0f
	},
		(Vector3) {
		0.5f, 0.5f, 0.5f
	}, RED, PROP_VISIBILE | PROP_COLLIDER | PROP_PICKUP | PROP_INTERACTABLE);
	props->interactType[testCube] = INTERACTABLE_PICKUP;

	testCube = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE,
		(Vector3) {
		4.75f, 1.8f, -9.0f
	},
		(Vector3) {
		0.5f, 0.5f, 0.5f
	}, PURPLE, PROP_VISIBILE | PROP_COLLIDER | PROP_PICKUP | PROP_INTERACTABLE);
	props->interactType[testCube] = INTERACTABLE_PICKUP;

    AddKillFlame((Vector3){20.0f, 0.0f, 0.0f}, (Vector3){1.0f, 1.0f, 1.0f}, true);
	AddKillFlame((Vector3){30.0f, 0.0f, 0.0f}, (Vector3){1.0f, 1.0f, 1.0f}, false);

    testCube = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) {40.0f, 1.8f, 0.0f}, 
        (Vector3) {1.0f, 1.0f, 1.0f}, RAYWHITE, PROP_VISIBILE | PROP_COLLIDER | PROP_PICKUP | PROP_INTERACTABLE | PROP_VENTLID);
	props->interactType[testCube] = INTERACTABLE_PICKUP;
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

        if (props->prim[i] == NO_PRIM)
        {
            snprintf(rec.modelPath, PROP_MODEL_PATH_MAX, props->modelPath[i]);
            snprintf(rec.texturePath, PROP_MODEL_PATH_MAX, props->texPath[i]);
        }
        else 
        {
            rec.modelPath[i] = '\0';
            rec.texturePath[i] = '\0';
        }
        rec.modelPath[PROP_MODEL_PATH_MAX - 1] = '\0';
        rec.texturePath[PROP_MODEL_PATH_MAX - 1] = '\0';
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

        if (props->prim[i] == NO_PRIM)
        {
            snprintf(rec.modelPath, PROP_MODEL_PATH_MAX, props->modelPath[i]);
            snprintf(rec.texturePath, PROP_MODEL_PATH_MAX, props->texPath[i]);
        }
        else 
        {
            rec.modelPath[i] = '\0';
            rec.texturePath[i] = '\0';
        }

        rec.modelPath[PROP_MODEL_PATH_MAX - 1] = '\0';
        rec.texturePath[PROP_MODEL_PATH_MAX - 1] = '\0';
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
    if (!props || !rec) return -1;

    if (rec->prim == NO_PRIM && rec->modelPath[0] != '\0')
    {
        /*Model model = LoadModel(rec->modelPath);
        if (&model == NULL)
        {
            TraceLog(LOG_WARNING, "Failed to load model '%s'. Using fallback cube.", rec->modelPath);
            model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
        }
        else if (rec->texturePath[0] != '\0')
        {
            Texture2D tex = LoadTexture(rec->texturePath);
            if (&tex == NULL)
            {
                model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;
            }
            else
            {
                TraceLog(LOG_WARNING, "Failed to load texture '%s' for model '%s'.", rec->texturePath, rec->modelPath);
            }
        }

        props->model[props->count] = model;*/
        int id = CreatePropFromPath(props, rec->modelPath, rec->texturePath, rec->position, rec->size, rec->color, rec->components);
        if (id >= 0)
        {
            props->prim[id] = NO_PRIM;
            strncpy(props->modelPath[id], rec->modelPath, PROP_MODEL_PATH_MAX);
            props->modelPath[id][PROP_MODEL_PATH_MAX - 1] = '\0';
            strncpy(props->texPath[id], rec->texturePath, PROP_MODEL_PATH_MAX);
            props->texPath[id][PROP_MODEL_PATH_MAX - 1] = '\0';
        }
        return id;
    }

    if (rec->prim == PRIMITIVE_MODEL_LENS)
    {
        return CreateLensProp(props, rec->position, rec->size);
    }

    if (rec->prim != NO_PRIM)
    {
        return CreatePropPrimitive(props, rec->prim, rec->position, rec->size, rec->color, rec->components);
    }

    Model fallback = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    return CreateProp(props, &fallback, rec->position, rec->size, rec->color, rec->components);
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