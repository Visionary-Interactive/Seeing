#include "map.h"

static char* pillar;
static char* book;
static char* wall;
static char* wall2;
static char* chair;
static char* wTexP;
static char* bTexP;
static char* pTexP;

bool InitMap(Map *map, const char *mapPath)
{
    pillar = "resources/global/models/pillar/scene.gltf";
    wTexP = "resources/global/models/pillar/textures/Material_baseColor.png";

    book = "resources/global/models/book/scene.gltf";
    Texture2D bookTexture = LoadTexture("resources/global/models/book/textures/01_-_Default_baseColor.png");
    bTexP = "resources/global/models/book/textures/01_-_Default_baseColor.png";

	chair = "resources/assets/chair.glb";

    wall = "resources/global/models/wall/wall.glb";
    wall2 = "resources/global/models/wall2/wall2.glb";

    return true;
}

void DrawMap(void)
{
}

void LoadLevel2 (Props* props)
{


    CreatePropFromPath(props, pillar, wTexP,
        (Vector3) {
        40.0, 0.0, 40.0
    }, (Vector3) { 0.15, 0.15, 0.15 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, pillar, wTexP,
        (Vector3) {
        -40.0, 0.0, 40.0
    }, (Vector3) { 0.15, 0.15, 0.15 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, pillar, wTexP,
        (Vector3) {
        40.0, 0.0, -40.0
    }, (Vector3) { 0.15, 0.15, 0.15 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, pillar, wTexP,
        (Vector3) {
        -40.0, 0.0, -40.0
    }, (Vector3) { 0.15, 0.15, 0.15 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);


    GenerateCubePuzzle(props, 3, 15, 50, -10, 10, (Vector3) { 1.0f, 1.0f, 1.0f }, YELLOW, PROP_VISIBILE);


	int newWallID = CreatePropFromPath(props, wall, wTexP, (Vector3) { 0.0f, 0.0f, 15.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
	CreatePropFromPath(props, wall, wTexP, (Vector3) { 0.0f, 0.0f, -2.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 0.0f, 0.0f, -12.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);
	CreatePropFromPath(props, wall, wTexP, (Vector3) { 0.0f, 0.0f, 25.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);


    CreatePropFromPath(props, wall2, wTexP, (Vector3) { -4.0f, 0.0f, -17.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { -14.0f, 0.0f, -17.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { -20.0f, 0.0f, -22.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, ORANGE, PROP_VISIBILE | PROP_COLLIDER);

    //Main Room
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { -4.0f, 0.0f, 30.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, RED, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { -14.0f, 0.0f, 30.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
	CreatePropFromPath(props, wall, wTexP, (Vector3) { -20.0f, 0.0f, 35.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);

    CreatePropFromPath(props, wall, wTexP, (Vector3) { -26.0f, 0.0f, 35.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);


    //Hallway
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 6.0f, 0.0f, 10.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 16.0f, 0.0f, 10.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 20.0f, 0.0f, 15.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 26.0f, 0.0f, 20.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 36.0f, 0.0f, 20.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 40.0f, 0.0f, 15.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 40.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 40.0f, 0.0f, -5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 36.0f, 0.0f, -8.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 26.0f, 0.0f, -8.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 20.0f, 0.0f, -3.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);


    CreatePropFromPath(props, wall, wTexP, (Vector3) { 20.0f, 3.0f, 15.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 26.0f, 3.0f, 20.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 36.0f, 3.0f, 20.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 40.0f, 3.0f, 15.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 40.0f, 3.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 40.0f, 3.0f, -5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 36.0f, 3.0f, -8.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 26.0f, 3.0f, -8.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall, wTexP, (Vector3) { 20.0f, 3.0f, -3.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, GREEN, PROP_VISIBILE | PROP_COLLIDER);


	CreatePropPrimitive(props, PRIMITIVE_MODEL_PLATFORM, (Vector3) { 32.0f, 50.0f, 6.0f }, (Vector3) { 15.0f, 1.0f, 15.0f }, BLACK, PROP_VISIBILE | PROP_COLLIDER);





    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 6.0f, 0.0f, 2.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP, (Vector3) { 16.0f, 0.0f, 2.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
   
}

void LoadLevel4(Props* props)
{
    char* wall1 = "resources/global/models/wall/wall1.glb";
    char* flatCeiling = "resources/global/models/ceiling/FlatCeiling.glb";

    // Back Walls
    int wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 0.0f, 0.0f, -15.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f + PI;
    ColliderSetup(props, wallTemp);
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -8.0f, 0.0f, -15.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f + PI;
    ColliderSetup(props, wallTemp);
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -16.0f, 0.0f, -15.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f + PI;
    ColliderSetup(props, wallTemp);

    // Room 1
    for (int i = 0; i < 9; i++)
    {
        // Walls
        CreatePropFromPath(props, wall1, wall1, (Vector3) { -18.5f, 0.0f, 54.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 5.0f, 0.0f, 54.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        props->rotation[wallTemp].y = PI;

        // Ceiling
        CreatePropFromPath(props, flatCeiling, flatCeiling, (Vector3) { -3.0f, -1.7f, 54.0f - (i * 9) }, (Vector3) { 6.0f, 6.0f, 6.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        CreatePropFromPath(props, flatCeiling, flatCeiling, (Vector3) { -11.0f, -1.7f, 54.0f - (i * 9) }, (Vector3) { 6.0f, 6.0f, 6.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        CreatePropFromPath(props, flatCeiling, flatCeiling, (Vector3) { -19.0f, -1.7f, 54.0f - (i * 9) }, (Vector3) { 6.0f, 6.0f, 6.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    }
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 0.0f, 0.0f, 59.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f;
    ColliderSetup(props, wallTemp);
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -8.0f, 0.0f, 59.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f;
    ColliderSetup(props, wallTemp);
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -16.0f, 0.0f, 59.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f;
    ColliderSetup(props, wallTemp);

	// Impairment Zone ---------------------------------------------------------------
    int zoneID = AddZone(props, (Vector3) { -5.0f, 0.0f, 25.0f }, (Vector3) { 30.0f, 4.0f, 1.0f }, TRIGGER_IMPAIRMENT);
    props->ImpairmentType[zoneID] = 1;
    props->lightIntensity[zoneID] = 1.9f;

    int zoneID2 = AddZone(props, (Vector3) { -5.0f, 0.0f, 25.0f }, (Vector3) { 30.0f, 4.0f, 1.0f }, TRIGGER_TEXT);
    props->textType[zoneID2] = TEXTBOX_PLAYER;
    props->text[zoneID2] = strdup("My eyes! It's getting dark... I can barely see.");

    // Books --------------------------------------------------------------------------
    // 1st
    int book = CreatePropFromPath(props, "resources/global/models/book/scene.gltf", "resources/global/models/book/textures/01_-_Default_baseColor.png",
        (Vector3) {
        0.5f, 1.0f, 45.0f
    }, (Vector3) { 0.03, 0.03, 0.03 },
            WHITE, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
    props->interactType[book] = INTERACTABLE_TEXT;
    props->textType[book] = TEXTBOX_BOOK;
    props->text[book] = strdup("A room with a shape matching puzzle. What could it mean?");
    // Pillar
    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        -2.0f, 0.0f, 20.0f
    }, (Vector3) { 0.02, 0.0033, 0.02 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);

    // 2nd
    book = CreatePropFromPath(props, "resources/global/models/book/scene.gltf", "resources/global/models/book/textures/01_-_Default_baseColor.png",
        (Vector3) {
        -2.0f, 1.0f, 20.0f
    }, (Vector3) { 0.03, 0.03, 0.03 },
            WHITE, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
    props->interactType[book] = INTERACTABLE_TEXT;
    props->textType[book] = TEXTBOX_BOOK;
    props->text[book] = strdup("There seems to be some trickery at play here... There has to be more shapes to find.\n Perhaps hidden in the grass somewhere...");
    // Pillar
    CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
        (Vector3) {
        0.5f, 0.0f, 45.0f
    }, (Vector3) { 0.02, 0.0033, 0.02 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);

	// Rotating Blocks ----------------------------------------------------------------
    // Wall - Disappearing
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 0.0f, 0.0f, 35.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f + PI;
    ColliderSetup(props, wallTemp);
	props->interactType[wallTemp] = INTERACTABLE_DISAPPEARING_WALL;
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -8.0f, 0.0f, 35.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f + PI;
    ColliderSetup(props, wallTemp);
    props->interactType[wallTemp] = INTERACTABLE_DISAPPEARING_WALL;
    wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -16.0f, 0.0f, 35.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f + PI;
    ColliderSetup(props, wallTemp);
    props->interactType[wallTemp] = INTERACTABLE_DISAPPEARING_WALL;

    char* blockLetters[3];
    for (int i = 0; i < 3; i++)
    {
		int puzzleBlock1 = CreatePropFromPath(props,
			"resources/global/models/puzzleBlock/PuzzleBlock1.glb",
			"resources/global/models/puzzleBlock/PuzzleBlock1.glb",
			(Vector3) {
			-10.5f + i * 5, 3.0f, 37.0f
		},
			(Vector3) {
			1.5f, 1.5f, 1.5f
		},
			WHITE, PROP_VISIBILE | PROP_INTERACTABLE);
		props->interactType[puzzleBlock1] = INTERACTABLE_PUZZLE_ROTATATION_BLOCK;
        // Pillar
        CreatePropFromPath(props, "resources/global/models/pillar/scene.gltf", "resources/global/models/pillar/textures/Material_baseColor.png",
            (Vector3) {
            -10.5f + i * 5, 0.0f, 37.0f
        }, (Vector3) { 0.02, 0.0072, 0.02 }, WHITE, PROP_VISIBILE | PROP_COLLIDER);

		//pre-compiler
		blockLetters[i] = calloc(255, sizeof(char));
		blockLetters[i][0] = (char)('0' + i);
		blockLetters[i][1] = ',';
		blockLetters[i][2] = 'd';
		blockLetters[i][3] = '\0';
		props->text[puzzleBlock1] = blockLetters[i]; // Set this so we can tell where the block is facing.
		props->rotation[puzzleBlock1].y = 0.0f + (90.0f * i);
        rotatingBlockIDs[i] = puzzleBlock1;
    }
}

void GenerateCubePuzzle(Props* props, int count, float minX, float maxX, float minZ, float maxZ, Vector3 size, Color color, uint32_t components)
{
    for (int i = 0; i < count; i++)
    {
        float x = (float)GetRandomValue((int)(minX * 100), (int)(maxX * 100)) / 100.0f;
        float z = (float)GetRandomValue((int)(minZ * 100), (int)(maxZ * 100)) / 100.0f;

        Vector3 position = { x, 49.0f, z }; // sit on ground

        CreatePropPrimitive(
            props,
            PRIMITIVE_MODEL_CUBE,
            position,
            size,
            color,
            components
        );
    }
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

	//int newWallID = CreatePropFromPath(props, wall2, wTexP, (Vector3) { 0.0f, 0.0f,-6.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BLUE, PROP_VISIBILE | PROP_COLLIDER);
	//big room back wall2 leftside
   /* CreatePropFromPath(props, wall2, wTexP,(Vector3) { -8.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP,(Vector3) { -18.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
    CreatePropFromPath(props, wall2, wTexP,(Vector3) { -28.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);*/
	//big room back wall rightside
	//CreatePropFromPath(props, wall2, wTexP,(Vector3) { 10.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
 //   CreatePropFromPath(props, wall2, wTexP, (Vector3) { 20.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);
 //   CreatePropFromPath(props, wall2, wTexP, (Vector3) { 26.0f, 0.0f, -32.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, BEIGE, PROP_VISIBILE | PROP_COLLIDER);


    //hallway leftside

    //hallway
    char* wall1 = "resources/global/models/wall/wall1.glb";
	char* curvedCeiling = "resources/global/models/ceiling/ceiling1.glb";
    int wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 0.0f, 0.0f,-5.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
    props->rotation[wallTemp].y = PI / 2.0f + PI;
	ColliderSetup(props, wallTemp);

    for (int i = 0; i < 7; i++)
    {
        CreatePropFromPath(props, curvedCeiling, curvedCeiling, (Vector3) { -5.0f, -2.0f, 54.0f - (i * 9) }, (Vector3) { 7.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        CreatePropFromPath(props, wall1, wall1, (Vector3) { -4.5f, 0.0f, 54.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 5.5f, 0.0f, 54.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        props->rotation[wallTemp].y = PI;

	}
	wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 0.0f, 0.0f, 59.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
	props->rotation[wallTemp].y = PI / 2.0f;
	ColliderSetup(props, wallTemp);

    // Big room
	// Left & Right side walls
    for (int i = 0; i < 5; i++)
    {
        // Left side
        CreatePropFromPath(props, wall1, wall1, (Vector3) { -33.5f, 0.0f, -32.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        CreatePropFromPath(props, wall1, wall1, (Vector3) { -33.5f, 5.0f, -32.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        // Ride side
        wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 32.5f, 0.0f, -32.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        props->rotation[wallTemp].y = PI;
        wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 32.5f, 5.0f, -32.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        props->rotation[wallTemp].y = PI;
        //wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { 5.5f, 0.0f, 54.0f - (i * 9) }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        //props->rotation[wallTemp].y = PI;

	}

    // Forward & Back walls
	char* flatCeiling = "resources/global/models/ceiling/FlatCeiling.glb";
	for (int i = 0; i < 9; i++)
	{
		//if (i == 4) continue; // Skip middle wall for door
		// Back walls
		wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -28.0f + (i * 9), 0.0f, -32.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
		props->rotation[wallTemp].y = PI / 2.0f;
		ColliderSetup(props, wallTemp);
		wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -28.0f + (i * 9), 5.0f, -32.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
		props->rotation[wallTemp].y = PI / 2.0f;
		ColliderSetup(props, wallTemp);

		// Forward walls
        wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -28.0f + (i * 9), 0.0f, -58.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
		props->rotation[wallTemp].y = PI / 2.0f + PI;
		ColliderSetup(props, wallTemp);
		wallTemp = CreatePropFromPath(props, wall1, wall1, (Vector3) { -28.0f + (i * 9), 5.0f, -58.0f }, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
		props->rotation[wallTemp].y = PI / 2.0f + PI;
		ColliderSetup(props, wallTemp);

        // Ceiling
        CreatePropFromPath(props, flatCeiling, flatCeiling, (Vector3) { -35.0f + (i * 9), 3.3f, -37.0f }, (Vector3) { 6.0f, 6.0f, 6.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        CreatePropFromPath(props, flatCeiling, flatCeiling, (Vector3) { -35.0f + (i * 9), 3.3f, -49.0f }, (Vector3) { 6.0f, 6.0f, 6.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
        CreatePropFromPath(props, flatCeiling, flatCeiling, (Vector3) { -35.0f + (i * 9), 3.3f, -61.0f }, (Vector3) { 6.0f, 6.0f, 6.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER);
	}


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

    //pushable cubes for the main puzzle
	int pushCubeID = CreatePropFromPath(props, "resources/global/models/pushBoulder/Push_Boulder.glb", "resources/global/models/pushBoulder/Push_Boulder.glb"
		, (Vector3) { -19.0f, 2.0f, -46.0f }, (Vector3) { 2.0f, 2.0, 2.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);
	int pushCubeID2 = CreatePropFromPath(props, "resources/global/models/pushBoulder/Push_Boulder.glb", "resources/global/models/pushBoulder/Push_Boulder.glb"
		, (Vector3) { 11, 2.0f, -46.0f }, (Vector3) { 2.0f, 2.0, 2.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PUSHABLE);
	props->interactType[pushCubeID] = INTERACTABLE_PUSH;
	props->interactType[pushCubeID2] = INTERACTABLE_PUSH;

	//starting platform for first room
	CreatePropFromPath(props, "resources/global/models/platform/platform2.glb", "resources/global/models/platform/platform2.glb",
		(Vector3) {
		-30, 0.5f, -56.0f
	},
		(Vector3) {
		3.0f, 3.0f, 3.0f
	}, WHITE, PROP_VISIBILE | PROP_COLLIDER);

	//middle platform with book
	CreatePropFromPath(props, "resources/global/models/platform/platform1.glb", "resources/global/models/platform/platform1.glb",
		(Vector3) {
		0.5, 2.5f, -35.0f
	},
		(Vector3) {
		6.0f, 3.0f, 3.0f
	}, WHITE, PROP_VISIBILE | PROP_COLLIDER);

	// End platform
	CreatePropFromPath(props, "resources/global/models/platform/platform1.glb", "resources/global/models/platform/platform1.glb",
		(Vector3) {
		29.0f, 2.5f, -56.0f
	},
		(Vector3) {
		3.0f, 3.0f, 3.0f
	}, WHITE, PROP_VISIBILE | PROP_COLLIDER);

	int doorID = CreatePropPrimitive(props, PRIMITIVE_MODEL_DOOR, (Vector3) { 29.0f, 5.0f, -56.0f },
		(Vector3) {
		1.0f, 1.0f, 1.0f
	}, GREEN, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_DOOR); //holy hell change this
	props->interactType[doorID] = INTERACTABLE_DOOR; //this sucks


	int pickupID = CreatePropFromPath(props, "resources/global/models/book/scene.gltf", "resources/global/models/book/textures/01_-_Default_baseColor.png",
		(Vector3) {
		1.5, 4.5f, -35.0f
	}, (Vector3) { 0.03, 0.03, 0.03 },
			WHITE, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
	props->interactType[pickupID] = INTERACTABLE_TEXT;
	props->textType[pickupID] = TEXTBOX_BOOK;
	props->text[pickupID] = strdup("Gained the ability to control Tritanopia!\nUse the UP and DOWN arrow keys to increase and decrease the Impairment\nI think I can solve this maze now!");

    int tutorialID = CreatePropFromPath(props, "resources/global/models/book/scene.gltf", "resources/global/models/book/textures/01_-_Default_baseColor.png",
        (Vector3) {
        2.5f, 1.0f, 50.0f
    }, (Vector3) { 0.03, 0.03, 0.03 },
            WHITE, PROP_VISIBILE | PROP_INTERACTABLE | PROP_PICKUP);
    props->interactType[tutorialID] = INTERACTABLE_TEXT;
    props->textType[tutorialID] = TEXTBOX_BOOK;
    props->text[tutorialID] = strdup("Oh gods, where am I? My legs still work - I can move with the WASD Keys.\nInteract with objects using the E key.\nI've got to get out of this tower!");

    int ventlidID = CreatePropFromPath(props, "resources/global/models/cauldron_lid/scene.gltf", "resources/global/models/cauldron_lid/textures/Material_baseColor.png",
        (Vector3) {
        -1.8f, 0.0f, 13.0f
    }, (Vector3) { 3.0f, 1.0f, 3.0f },
			WHITE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE |PROP_VENTLID);
	props->interactType[ventlidID] = INTERACTABLE_VENTLID;

    int ventlidID2 = CreatePropFromPath(props, "resources/global/models/cauldron_lid/scene.gltf", "resources/global/models/cauldron_lid/textures/Material_baseColor.png",
        (Vector3) {
        -0.6f, 0.0f, -56.0f
    }, (Vector3) { 3.0f, 1.0f, 3.0f },
            WHITE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_VENTLID);
    props->interactType[ventlidID2] = INTERACTABLE_VENTLID;

	int zoneID = AddZone(props, (Vector3) { 0.5f, 0.0f, 22.0f }, (Vector3) { 8.0f, 4.0f, 1.0f }, TRIGGER_IMPAIRMENT);
	props->ImpairmentType[zoneID] = 0;

    int zoneID2 = AddZone(props, (Vector3) { 0.5f, 0.0f, 22.0f }, (Vector3) { 8.0f, 4.0f, 1.0f }, TRIGGER_TEXT);
    props->textType[zoneID2] = TEXTBOX_PLAYER;
	props->text[zoneID2] = strdup("My eyes, what's happening? My vision is all strange...");

    int zoneID3 = AddZone(props, (Vector3) { 0.5f, 0.0f, 44.0f }, (Vector3) { 8.0f, 4.0f, 1.0f }, TRIGGER_TEXT);
    props->textType[zoneID3] = TEXTBOX_PLAYER;
    props->text[zoneID3] = strdup("Hah, what a trick! They intend to mimic a flame, but its green colour gives it away.");

	// Warp Zone
	int warpID = AddZone(props, (Vector3) { 0.0f, 0.0f, -4.0f }, (Vector3) { 2.0f, 5.0f, 2.0f }, TRIGGER_WARP);
	props->warpTarget[warpID] = (Vector3){ -30.0f, 4.0f, -56.0f };
    // Warp Pad Model
	CreatePropFromPath(props, "resources/global/models/warpPad/warpPad1.glb", "resources/global/models/warpPad/warpPad1.glb",
		(Vector3) { 0.0f, 0.0f, -3.5f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
	CreatePropFromPath(props, "resources/global/models/warpPad/warpPad1.glb", "resources/global/models/warpPad/warpPad1.glb",
		(Vector3) { -30.0f, 1.5f, -56.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);

	char* ventModel = "resources/global/models/vent/scene.gltf";
	char* ventTexture = "resources/global/models/vent/textures/Material_baseColor.png";

	//fires for hallway puzzle
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
    AddKillFlame((Vector3) { 4.0f, 0.0f, 10.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);

    AddKillFlame((Vector3) { -2.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 1.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, true);
    AddKillFlame((Vector3) { 4.0f, 0.0f, 5.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, false);

    //fires for main room puzzle

    AddKillFlame((Vector3) { -25.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -19.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -13.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -7.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -1.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 5.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 11.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 17.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 23.0f, 0.0f, -54.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
 

    AddKillFlame((Vector3) { -25.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -19.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -13.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -7.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -1.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 5.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 11.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 17.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 23.0f, 0.0f, -50.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);

    AddKillFlame((Vector3) { -25.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -19.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -13.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -7.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -1.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 5.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 11.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 17.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 23.0f, 0.0f, -46.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);

    AddKillFlame((Vector3) { -25.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -19.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { -13.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -7.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { -1.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 5.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 11.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, false);
    AddKillFlame((Vector3) { 17.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);
    AddKillFlame((Vector3) { 23.0f, 0.0f, -42.0f }, (Vector3) { 3.0f, 3.0f, 3.0f }, true);


    // Decorations ------------------------------------------------------------------------------------

    char* wallTorch = "resources/global/models/wallTorch/wallTorch1.glb";
	// Wall Torches for hallway
    for (int i = 0; i < 6; i++)
    {
        int torchTemp;
        torchTemp = CreatePropFromPath(props, wallTorch, wallTorch,
            (Vector3) { -3.4f, 2.0f, 43.0f + (i * -9.0f) }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
        props->rotation[torchTemp].y = PI / 2.0f;
        torchTemp = CreatePropFromPath(props, wallTorch, wallTorch,
            (Vector3) { 4.4f, 2.0f, 43.0f + (i * -9.0f) }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
        props->rotation[torchTemp].y = PI + PI / 2.0f;
	}

	// Wall Torches for big room
    for (int i = 0; i < 4; i++)
    {
        int torchTemp;
        torchTemp = CreatePropFromPath(props, wallTorch, wallTorch,
            (Vector3) { -32.3f, 6.0f, -62.0f + (i * 9.0f) }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
        props->rotation[torchTemp].y = PI / 2.0f;
        torchTemp = CreatePropFromPath(props, wallTorch, wallTorch,
            (Vector3) { 31.3f, 6.0f, -62.0f + (i * 9.0f) }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
        props->rotation[torchTemp].y = PI + PI / 2.0f;
	}
    for (int i = 0; i < 6; i++)
    {
        int torchTemp;
        torchTemp = CreatePropFromPath(props, wallTorch, wallTorch,
            (Vector3) { -22.0f + (i * 9.0f), 6.0f, -56.9f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
        torchTemp = CreatePropFromPath(props, wallTorch, wallTorch,
            (Vector3) { -22.0f + (i * 9.0f), 6.0f, -33.1f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
		props->rotation[torchTemp].y = PI;
	}

    // Vases
	char* vaseModel = "resources/global/models/vase/vase1.glb";
	int vaseTemp;

	// Vases for hallway
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { 2.7f, 0.0f, 56.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE); 
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { -2.1f, 0.0f, 55.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE); 
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { 3.1f, 0.0f, -2.8f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE); 
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { -2.7f, 0.0f, -3.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE); 
    props->rotation[vaseTemp].y = rand() % 360;

	// Vases for big room
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { -19.0f, 0.0f, -61.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
	props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { -2.7f, 0.0f, -35.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { -1.3f, 0.0f, -56.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { 15.0f, 0.0f, -44.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE);
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { 24.0f, 0.0f, -52.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE); 
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { 24.0f, 0.0f, -34.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE); 
    props->rotation[vaseTemp].y = rand() % 360;
    vaseTemp = CreatePropFromPath(props, vaseModel, vaseModel,
            (Vector3) { -25.0f, 0.0f, -37.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE); 
    props->rotation[vaseTemp].y = rand() % 360;

    // Grass
	char* grassModel = "resources/global/models/grass/grass1.glb";
	// Hallway grass
    for (int i = 0; i < 25; i++)
    {
        int maxX = 4;
		int minX = -3;
        int maxZ = 57;
		int minZ = 42;
        int grassTemp = CreatePropFromPath(props, grassModel, grassModel,
            (Vector3) { (float)(rand() % (maxX - minX + 1) + minX), -0.2f, (float)(rand() % (maxZ - minZ + 1) + minZ) }, (Vector3) { 1.0f, 1.0f, 1.0f },
            CLITERAL(Color){ 0, 117, 44, 150 }, PROP_VISIBILE);
        props->rotation[grassTemp].y = rand() % 360;
	}
    for (int i = 0; i < 15; i++)
    {
        int maxX = 4;
		int minX = -3;
        int maxZ = 38;
		int minZ = 32;
        int grassTemp = CreatePropFromPath(props, grassModel, grassModel,
            (Vector3) { (float)(rand() % (maxX - minX + 1) + minX), -0.2f, (float)(rand() % (maxZ - minZ + 1) + minZ) }, (Vector3) { 1.0f, 1.0f, 1.0f },
            CLITERAL(Color){ 0, 117, 44, 150 }, PROP_VISIBILE);
        props->rotation[grassTemp].y = rand() % 360;
	}
    for (int i = 0; i < 10; i++)
    {
        int maxX = 4;
		int minX = -3;
        int maxZ = 24;
		int minZ = 20;
        int grassTemp = CreatePropFromPath(props, grassModel, grassModel,
            (Vector3) { (float)(rand() % (maxX - minX + 1) + minX), -0.2f, (float)(rand() % (maxZ - minZ + 1) + minZ) }, (Vector3) { 1.0f, 1.0f, 1.0f },
            CLITERAL(Color){ 0, 117, 44, 150 }, PROP_VISIBILE);
        props->rotation[grassTemp].y = rand() % 360;
	}
    for (int i = 0; i < 10; i++)
    {
        int maxX = 4;
		int minX = -3;
        int maxZ = 16;
		int minZ = 12;
        int grassTemp = CreatePropFromPath(props, grassModel, grassModel,
            (Vector3) { (float)(rand() % (maxX - minX + 1) + minX), -0.2f, (float)(rand() % (maxZ - minZ + 1) + minZ) }, (Vector3) { 1.0f, 1.0f, 1.0f },
            CLITERAL(Color){ 0, 117, 44, 150 }, PROP_VISIBILE);
        props->rotation[grassTemp].y = rand() % 360;
	}
	// Big room grass
    for (int i = 0; i < 100; i++)
    {
        int maxX = 31;
		int minX = -32;
        int maxZ = -33;
		int minZ = -62;
        int grassTemp = CreatePropFromPath(props, grassModel, grassModel,
            (Vector3) { (float)(rand() % (maxX - minX + 1) + minX), -0.2f, (float)(rand() % (maxZ - minZ + 1) + minZ) }, (Vector3) { 1.0f, 1.0f, 1.0f },
            CLITERAL(Color){ 0, 117, 44, 150 }, PROP_VISIBILE);
        props->rotation[grassTemp].y = rand() % 360;
	}
	// Decorations End --------------------------------------------------------------------------------

	//AddDeadzone(props, (Vector3) { 0.0f, 0.0f, -20.0f }, (Vector3) { 10.0f, 10.0f, 10.0f });

    // Spawning rotating puzzle blocks for testing
    
    char* blockLetters[3];
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
		blockLetters[i] = calloc(2, sizeof(char));
        blockLetters[i][0] = (char)('0' + i);
		blockLetters[i][1] = '\0';
		props->text[puzzleBlock1] = blockLetters[i]; // Set this so we can tell where the block is facing.
		props->rotation[puzzleBlock1].y = 0.0f + (90.0f * i);
        rotatingBlockIDs[i] = puzzleBlock1;
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

    CreatePropFromPath(props, "resources/global/models/vent/scene.gltf", "resources/global/models/vent/textures/Material_baseColor.png",
        (Vector3) {
        0.0f, 0.0f, -20.0f
    }, (Vector3) { 1.0f, 1.0f, 1.0f }, WHITE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE);
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
    return CreateProp(props, &fallback, rec->position, rec->size, PURPLE, rec->components);
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