#include "includes.h"
#include "player.h"
#include "camera.h"
#include "impairment.h"
#include "map.h"


static void DrawLevel(void);

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "A Game About Seeing");

    //all this stuff should be toggleable
    DisableCursor(); //hide cursor for mouselook
    SetTargetFPS(120);
    SetConfigFlags(FLAG_MSAA_4X_HINT); 

    InitPlayer();
    Player* player = GetPlayer();


	Model doorModel = LoadModelFromMesh(GenMeshCube(3.0, 4.0, 0.5));

	//GameObject array to hold all objects in the scene
	GameObject* gameObjects = (GameObject*)malloc(sizeof(GameObject));
	memset(gameObjects, 0, sizeof(GameObject));

    //Create a Wall to test collision
    CreateObject(gameObjects,
        (Vector3) {0, 4.0, -7},
        (Vector3) {1.0, 1.0, 1.0},
        LoadModelFromMesh(GenMeshCube(1.0,1.0,1.0)), RED, OBJECT_VISIBILE | OBJECT_COLLIDER);

    
    //Create the door object to interact with
    int doorID = CreateObject(gameObjects,
        (Vector3) {0, 2.0f, -10.0f},
     (Vector3) {3.0f, 4.0f, 0.5f },doorModel, GREEN, OBJECT_VISIBILE | OBJECT_COLLIDER | OBJECT_INTERACTABLE | OBJECT_DOOR);
	gameObjects->interactType[doorID] = INTERACTABLE_DOOR;

    int pickupID = CreateObject(gameObjects,
        (Vector3) {-10, 4.0, 0},
        (Vector3) {1.0, 1.0, 1.0},
		LoadModelFromMesh(GenMeshCube(1.0,1.0,1.0)), BLUE, OBJECT_VISIBILE | OBJECT_COLLIDER | OBJECT_INTERACTABLE | OBJECT_PICKUP);
	gameObjects->interactType[pickupID] = INTERACTABLE_PICKUP;

    InitCamera();
	Camera* camera = GetCamera();

    Map gameMap;

    Impairment* astig = LoadImpairment(Astigmatism, screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        UpdatePlayer(gameObjects);
        UpdateImpairment(astig);

        RefreshCamera(player);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginImpairment(astig);

        BeginMode3D(*camera);
        DrawMap(&gameMap);
		RenderProps(gameObjects);
        EndMode3D();
        UpdateInteractions(gameObjects);
        EndImpairment(astig);
        DrawText("WASD to move, MOUSE to look, ESC to quit", 10, 10, 20, DARKGRAY);
        DrawText("SHIFT to sprint, SPACE to jump", 10, 40, 20, DARKGRAY);
        DrawText("Current Impairment Loaded: Astigmatism", 10, 70, 20, DARKGRAY);
        DrawText("[LEFT/RIGHT] to adjust angle, [UP/DOWN] to adjust intensity, [O/P] to swap presets", 10, 100, 20, DARKGRAY);
        DrawText(TextFormat("Player Position: (%.3f, %.3f, %.3f)",
            player->position.x, player->position.y, player->position.z),
            10, 130, 20, DARKGRAY);
        DrawText(TextFormat("Camera Target: (%.3f, %.3f, %.3f)",
            camera->target.x, camera->target.y, camera->target.z),
            10, 160, 20, DARKGRAY);
        EndDrawing();
    }

    DestroyImpairment(astig);
    DestroyCamera();
    DestroyPlayer();
    CloseWindow();
	free(gameObjects);
    return 0;
}


// CMD build and start debugging
// cd build && ./premake5 gmake && cd .. && make && ./bin/Debug/Seeing