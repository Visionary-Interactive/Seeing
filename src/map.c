#include "map.h"

void LoadMap(Props* props)
{
    Model doorModel = LoadModelFromMesh(GenMeshCube(3.0, 4.0, 0.5));

    //Create a Wall to test collision
	CreateProp(props,
		(Vector3) {
		0, 4.0, -7
	},
		(Vector3) {
		1.0, 1.0, 1.0
	},
		LoadModelFromMesh(GenMeshCube(1.0, 1.0, 1.0)), RED, PROP_VISIBILE | PROP_COLLIDER);


	//Create the door PROP to interact with
	int doorID = CreateProp(props,
		(Vector3) {
		0, 2.0f, -10.0f
	},
		(Vector3) {
		3.0f, 4.0f, 0.5f
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
}

//a Drawlevel code to implement level creation in its own function
void DrawMap()//const Map *map)
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