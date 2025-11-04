#include "includes.h"
#include "player.h"
#include "shader.h"

static void DrawLevel(void);
//creates a bounding box around the player for collision detection

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "A Game About Seeing");
    DisableCursor();   // Hide cursor for mouselook
    SetTargetFPS(60);

    InitPlayer();
    Player* player = GetPlayer();

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
        UpdatePlayer();
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
        DrawLevel();
        EndMode3D();

        EndVisionRender();
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
    DestroyVisionShader();
    DestroyPlayer();

    CloseWindow();
    return 0;
}

//a Drawlevel code to implement level creation in its own function
static void DrawLevel(void)
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

