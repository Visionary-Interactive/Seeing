#include "map.h"

static Model pillar;
static Model book;
static Model wall;
static Model wall2;
static Model door;
static Model chair;

bool InitMap(Map *map, const char *mapPath)
{
    pillar = LoadModel("resources/global/models/pillar/scene.gltf");
    Texture2D texture = LoadTexture("resources/global/models/pillar/textures/Material_baseColor.png");
    book = LoadModel("resources/global/models/book/scene.gltf");
    Texture2D bookTexture = LoadTexture("resources/global/models/book/textures/01_-_Default_baseColor.png");
	chair = LoadModel("resources/assets/chair.glb");
	wall = LoadModel("resources/assets/wall.glb");
	wall2 = LoadModel("resources/assets/wall2.glb");
    pillar.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	book.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    return true;
}

void DrawMap(void)
{
}

void LoadPropTest(Props* props)
{
    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3){40.0, 0.0, 40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3){-40.0, 0.0, 40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3){40.0, 0.0, -40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3){-40.0, 0.0, -40.0}, (Vector3) {0.15, 0.15, 0.15}, WHITE, PROP_VISIBILE | PROP_COLLIDER);

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

	//Create the door PROP to interact with

    CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR,
        (Vector3) {
        -4.0f, 2.0f, -10.0f
    },
        (Vector3) {
        1.0, 1.0, 1.0
    }, GREEN, PROP_VISIBILE | PROP_COLLIDER);

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
		1.0f, 1.5f, 1.0f
	}, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

    doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) { 0.0f, 2.0f, -10.0f },
        (Vector3) {
        1.0f, 1.0f, 1.0f
    }, GREEN, PROP_VISIBILE | PROP_COLLIDER);

    doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) { 4.0f, 2.0f, -10.0f },
        (Vector3) {
        1.0f, 1.0f, 1.0f
    }, GREEN, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_DOOR); //holy hell change this
    props->interactType[doorID] = INTERACTABLE_DOOR; //this sucks

	int pushCubeID = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) { -10.0f, 2.0f, -5.0f }, (Vector3) { 4.0f, 4.0f, 4.0f }, RED, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);
	props->interactType[pushCubeID] = INTERACTABLE_PUSH;

	int pickupID = CreateProp(props, book, 
        (Vector3) {-10, 1.0, 0}, (Vector3) {0.03, 0.03, 0.03}, 
        WHITE, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
	props->interactType[pickupID] = INTERACTABLE_TEXT; //this also sucks

    int pickupID2 = CreateProp(props, book,
        (Vector3) {
        -15, 1.0, 0
    }, (Vector3) { 0.03, 0.03, 0.03 },
            RED, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
    props->interactType[pickupID2] = INTERACTABLE_PICKUP;

    int pickupID3 = CreateProp(props, book,
        (Vector3) {
        -25, 1.0, 0
    }, (Vector3) { 0.03, 0.03, 0.03 },
            GREEN, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
    props->interactType[pickupID3] = INTERACTABLE_PICKUP;

    Vector3 lensPos  = (Vector3){ 30.0f, 2.0f, 40.0f };
    Vector3 lensSize = (Vector3){ 2.0f, 4.0f, 2.0f };
    CreateLensProp(props, lensPos, lensSize);

    
*/


	int newWallID = CreateProp(props, wall2, (Vector3) { 0.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	//big room back wall2 leftside
    CreateProp(props, wall2, (Vector3) { -8.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall2, (Vector3) { -18.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall2, (Vector3) { -28.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	//big room back wall rightside
	CreateProp(props, wall2, (Vector3) { 10.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall2, (Vector3) { 20.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall2, (Vector3) { 26.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

	int newWallID2 = CreateProp(props, wall, (Vector3) { -4.0f, 0.0f, 0.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    //hallway leftside
	CreateProp(props, wall, (Vector3) { -4.0f, 0.0f, -9.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	CreateProp(props, wall, (Vector3) { -4.0f, 0.0f, -18.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { -4.0f, 0.0f, -27.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

	//big room leftside
    CreateProp(props, wall, (Vector3) { -33.5f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { -33.5f, 0.0f, -37.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { -33.5f, 0.0f, -42.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { -33.5f, 0.0f, -47.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { -33.5f, 0.0f, -52.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

    //hallway rightside
    CreateProp(props, wall, (Vector3) { 4.0f, 0.0f, 0.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	CreateProp(props, wall, (Vector3) { 4.0f, 0.0f, -9.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { 4.0f, 0.0f, -18.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { 4.0f, 0.0f, -27.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

    //big room rightside
    CreateProp(props, wall, (Vector3) { 30.5f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { 30.5f, 0.0f, -37.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { 30.5f, 0.0f, -42.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { 30.5f, 0.0f, -47.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreateProp(props, wall, (Vector3) { 30.5f, 0.0f, -52.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

    int pushCubeID = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) { -19.0f, 2.0f, -45.0f }, (Vector3) { 4.0f, 4.0f, 4.0f }, RED, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);
    int pushCubeID2 = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) { 5, 2.0f, -47.0f }, (Vector3) { 4.0f, 4.0f, 4.0f }, ORANGE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);
    int pushCubeID3 = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, (Vector3) { 12, 2.0f, -55.0f }, (Vector3) { 4.0f, 4.0f, 4.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);

    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        -15.0f, 1.5f, -40.0f
    },
        (Vector3) {
        1.0f, 1.5f, 1.0f
    }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        -20.0f, 1.8f, -40.0f
    },
        (Vector3) {
        1.0f, 1.5f, 1.0f
    }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);


    CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM,
        (Vector3) {
        29.0f, 3.5f, -57.0f
    },
        (Vector3) {
        1.0f, 1.5f, 1.0f
    }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

    int doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) { 29.0f, 6.0f, -57.0f },
        (Vector3) {
        1.0f, 1.0f, 1.0f
    }, GREEN, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_DOOR); //holy hell change this
    props->interactType[doorID] = INTERACTABLE_DOOR; //this sucks


    props->interactType[pushCubeID] = INTERACTABLE_PUSH;
	props->interactType[pushCubeID2] = INTERACTABLE_PUSH;
	props->interactType[pushCubeID3] = INTERACTABLE_PUSH;


	//int chairID = CreateProp(props, chair, (Vector3) { 5.5f, 0.0f, -1.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BROWN, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE);

    int pickupID = CreateProp(props, book,
        (Vector3) {
        -0.8, 1.0, 3.5f
    }, (Vector3) { 0.03, 0.03, 0.03 },
            WHITE, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
    props->interactType[pickupID] = INTERACTABLE_TEXT;
	props->textType[pickupID] = TEXTBOX_BOOK;
	props->text[pickupID] = "Where am I, I don't know what's going on! \n My feet still seem to work (WASD) and I can Jump pretty well.(Space) \n I need to Get out of here";

	//AddDeadzone(props, (Vector3) { 0.0f, 0.0f, -20.0f }, (Vector3) { 10.0f, 10.0f, 10.0f });


}

/*void DrawFloor()//const Map *map)
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
}*/

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
            snprintf(rec.modelPath, PROP_MODEL_PATH_MAX, "resources/global/models/pillar/scene.gltf");
            snprintf(rec.texturePath, PROP_MODEL_PATH_MAX, "resources/global/models/pillar/textures/Material_baseColor.png");
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
            snprintf(rec.modelPath, PROP_MODEL_PATH_MAX, "resources/global/models/pillar/scene.gltf");
            snprintf(rec.texturePath, PROP_MODEL_PATH_MAX, "resources/global/models/pillar/textures/Material_baseColor.png");
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
    return CreateProp(props, fallback, rec->position, rec->size, rec->color, rec->components);
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