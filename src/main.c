#include "includes.h"
#include "player.h"
#include "camera.h"
#include "impairment.h"
#include "map.h"
#include "shader.h"
#include "SessionManager.h"
#include <time.h>

#define SERVER_PORT 12345
#define TICK_RATE_MS 20


static void DrawLevel(void);

int main(int argc, char** argv)
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

    // Set up Server
	int ev; // event variable
    if (argc < 2)
    {
        printf("Usage: %s <server|client> [host-for-client]\n", argv[0]);
        return 1;
    }

    SessionManager_Init();
    if (strcmp(argv[1], "server") == 0)
    {
        SessionManager_CreateServer("UDP", SERVER_PORT);
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        const char* host = (argc >= 3) ? argv[2] : "127.0.0.1"; // set default host to localhost
        SessionManager_CreateClient("UDP", host, SERVER_PORT);
    }

    clock_t last_network_tick = clock();

    while (!WindowShouldClose())
    {
        UpdatePlayer(gameObjects);
        UpdateImpairment(astig);
		// Handle networking events at fixed tick rate
		clock_t now = clock();
        int elapsed_ms = (int)(now - last_network_tick);

        if (elapsed_ms >= TICK_RATE_MS)
        {
            last_network_tick = now;

            if (strcmp(argv[1], "server") == 0)
            {
                ev = SessionManager_Server_HandleEvents();

                struct Snapshot playerPosition;
                playerPosition.forward = IsKeyDown(KEY_W);
                playerPosition.backward = IsKeyDown(KEY_S);
                playerPosition.left = IsKeyDown(KEY_A);
                playerPosition.right = IsKeyDown(KEY_D);
                playerPosition.y = player->position.y;

                SessionManager_Server_Tick(playerPosition);
                SessionManager_Server_SendPackets();
            }
            else if (strcmp(argv[1], "client") == 0)
            {
                ev = SessionManager_Client_HandleEvents();

                player->ex_W = lastSnapshot.forward;
                player->ex_S = lastSnapshot.backward;
                player->ex_A = lastSnapshot.left;
                player->ex_D = lastSnapshot.right;

                SessionManager_Client_SendPackets();
            }
        }


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

	// Stop Server/Client
    if (strcmp(argv[1], "server") == 0)
        SessionManager_StopServer();
    else if (strcmp(argv[1], "client") == 0)
        SessionManager_StopClient();

    CloseWindow();
	free(gameObjects);
    return 0;
}


// CMD build and start debugging
// cd build && ./premake5 gmake && cd .. && make && ./bin/Debug/Seeing