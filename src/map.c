#include "map.h"
#include "lens.h"

void LoadPropTest(Props* props)
{
    Model doorModel = LoadModelFromMesh(GenMeshCube(3.0, 4.0, 0.5));

    //Create a Wall to test collision
	CreateProp(props,
		(Vector3) {
		0.0f, 4.0, -7.0f
	},
		(Vector3) {
		6.0, 4.0, 1.0
	},
		LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), RED, PROP_VISIBILE | PROP_COLLIDER);


	//Create the door PROP to interact with
	int doorID = CreateProp(props,
		(Vector3) {
		0, 2.0f, -10.0f
	},
		(Vector3) {
		3.0f, 1.0f, 0.5f
	}, doorModel, GREEN, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_DOOR);
	props->interactType[doorID] = INTERACTABLE_DOOR;

	int pickupID = CreateProp(props,
		(Vector3) {
		-10, 4.0, 0
	},
		(Vector3) {
		1.0, 1.0, 1.0
	},
		LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), BLUE, PROP_VISIBILE | PROP_COLLIDER | PROP_INTERACTABLE | PROP_PICKUP);
	props->interactType[pickupID] = INTERACTABLE_PICKUP;

    Vector3 lensPos  = (Vector3){ 3.25f, 2.0f, -2.1f };
    Vector3 lensSize = (Vector3){ 2.0f, 4.0f, 2.0f };
    CreateLensProp(props, lensPos, lensSize);

    lensPos  = (Vector3){ -3.25f, 2.0f, -20.1f };
    lensSize = (Vector3){ 2.0f, 4.0f, 2.0f };
    CreateLensProp(props, lensPos, lensSize);
}

//a Drawlevel code to implement level creation in its own function
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
    Vector3 towerPos = (Vector3){ 16.0f, 16.0f, 16.0f };
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

void SaveMap(Map *map, const char *mapPath)
{
    FILE *f = fopen(TextFormat("%s/map.bin", mapPath), "wb");
    if (!f) {
        TraceLog(LOG_ERROR, "Failed to open map file for writing!");
        return;
    }

    //write game version
    uint8_t version[2] = { MAJOR_VERSION, MINOR_VERSION };
    fwrite(version, sizeof(uint8_t), 2, f);

    Props *props = GetPropStructure();

    //write the prop count
    uint32_t count = (uint32_t)props->count;
    fwrite(&count, sizeof(uint32_t), 1, f);

    for (size_t i = 0; i < props->count; i++) {
        fwrite(&props->position[i], sizeof(Vector3), 1, f);
        fwrite(&props->size[i], sizeof(Vector3), 1, f);
        fwrite(&props->color[i], sizeof(Color), 1, f);
        fwrite(&props->interactRange[i], sizeof(Vector3), 1, f);
        fwrite(&props->lightColor[i], sizeof(Color), 1, f);
        fwrite(&props->lightIntensity[i], sizeof(float), 1, f);
        fwrite(&props->components[i], sizeof(uint32_t), 1, f);
        fwrite(&props->interactType[i], sizeof(int32_t), 1, f);
        fwrite(&props->scriptID[i], sizeof(int32_t), 1, f);

        //model path todo
        /*uint16_t len = (uint16_t)strlen(props->modelPath[i]);
        fwrite(&len, sizeof(uint16_t), 1, f);
        fwrite(props->modelPath[i], sizeof(char), len, f);*/
    }

    fclose(f);
    TraceLog(LOG_INFO, "Map saved successfully.");
}

void LoadMapFile(const char *mapPath)
{
    FILE *f = fopen(TextFormat("%s/map.bin", mapPath), "rb");
    if (!f) {
        TraceLog(LOG_ERROR, "Failed to open map file for loading!");
        return;
    }

    uint8_t version[2];
    fread(version, sizeof(uint8_t), 2, f);

	if (version[0] <= MAJOR_VERSION && version[1] < MINOR_VERSION) {
        TraceLog(LOG_WARNING, "This map was designed in an older version (%u,%u), some elements may not work correctly",
                 version[0], version[1]);
    }

    Props *props = GetPropStructure();
    memset(props, 0, sizeof(Props));

    uint32_t count = 0;
    fread(&count, sizeof(uint32_t), 1, f);

    props->count = count;

    for (size_t i = 0; i < count; i++) {
        fread(&props->position[i], sizeof(Vector3), 1, f);
        fread(&props->size[i], sizeof(Vector3), 1, f);
        fread(&props->color[i], sizeof(Color), 1, f);
        fread(&props->interactRange[i], sizeof(Vector3), 1, f);
        fread(&props->lightColor[i], sizeof(Color), 1, f);
        fread(&props->lightIntensity[i], sizeof(float), 1, f);
        fread(&props->components[i], sizeof(uint32_t), 1, f);
        fread(&props->interactType[i], sizeof(int32_t), 1, f);
        fread(&props->scriptID[i], sizeof(int32_t), 1, f);

        //model path todo
        /*uint16_t len;
        fread(&len, sizeof(uint16_t), 1, f);
        fread(props->modelPath[i], sizeof(char), len, f);
        props->modelPath[i][len] = '\0';

        props->model[i] = LoadModel(props->modelPath[i]);*/

        ColliderSetup(props, i);
    }

    fclose(f);
    TraceLog(LOG_INFO, "Map loaded successfully.");
}
