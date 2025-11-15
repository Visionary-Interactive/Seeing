#include "includes.h"
#include "player.h"
#include "object.h"
#include "shader.h"


static void DrawLevel(void);
void Endgame(void)
{
    DestroyVisionShader();
    DestroyPlayer();
    CloseWindow();
}

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "A Game About Seeing");
    DisableCursor();   // Hide cursor for mouselook
    SetTargetFPS(60);

    InitPlayer();
    Player* player = GetPlayer();


	//level dimensions
    Model wallModel = LoadModelFromMesh(GenMeshCube(20.0, 5.0, 1.0));
	Model wallModel2 = LoadModelFromMesh(GenMeshCube(1.0, 5.0, 20.0));
	Model doorModel = LoadModelFromMesh(GenMeshCube(3.0, 4.0, 0.5));

	//GameObject array to hold all objects in the scene
	GameObject* gameObjects = (GameObject*)malloc(sizeof(GameObject));
	memset(gameObjects, 0, sizeof(GameObject));

    //Create a Wall to test collision
    CreateObject(gameObjects,
        (Vector3) {0, 4.0, -7},
        (Vector3) {1.0, 1.0, 1.0},
        wallModel, RED,OBJECT_VISIBILE | OBJECT_COLLIDER);

    
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

    InitVisionShader(screenWidth, screenHeight);

    //Sets up camera
    Camera camera = { 0 };
    camera.position = player->position;
    camera.target = (Vector3){ 0.0f, 1.8f, 1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {


        //getting the direction the mosue is moving for the purposes of the camera
        UpdatePlayer(gameObjects);
        UpdateVisionShader(GetFrameTime());

        //allows the camera to follow the player's head based on rotation
        camera.position = player->position;
        camera.target.x = player->position.x + sinf(player->yaw) * cosf(player->pitch);
        camera.target.y = player->position.y + sinf(player->pitch);
        camera.target.z = player->position.z + cosf(player->yaw) * cosf(player->pitch);

   
		// starts to Draw the 3D world

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginVisionRender();

        BeginMode3D(camera);
        DrawCubeV(player->position, player->size, RED);
		RenderProps(gameObjects);
        DrawLevel();
        EndMode3D();

        EndVisionRender();
        UpdateInteractions(gameObjects);
        DrawText("WASD to move, MOUSE to look, ESC to quit", 10, 10, 20, DARKGRAY);
        DrawText("SHIFT to sprint, SPACE to jump", 10, 40, 20, DARKGRAY);
        DrawText("Current Impairment Loaded: Astigmatism", 10, 70, 20, DARKGRAY);
        DrawText("[LEFT/RIGHT] to adjust angle, [UP/DOWN] to adjust intensity, [O/P] to swap presets", 10, 100, 20, DARKGRAY);
        DrawText(TextFormat("Player Position: (%.3f, %.3f, %.3f)",
            player->position.x, player->position.y, player->position.z),
            10, 130, 20, DARKGRAY);
        EndDrawing();
    }

    //UnloadRenderTexture(target);

	free(gameObjects);
    Endgame();
    return 0;
}

//a Drawlevel code to implement level creation in its own function
static void DrawLevel()
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
}

