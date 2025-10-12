#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"
#include <math.h>

typedef struct Player {
    Vector3 position;
    float speed;
    float yaw;   // left/right rotation
    float pitch; // up/down rotation
} Player;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "A Game about Seeing");
    DisableCursor();   // Hide cursor for mouselook
    SetTargetFPS(60);

    // --- Player setup ---
    Player player = { 0 };
    player.position = (Vector3){ 0.0f, 1.8f, 0.0f };
    player.speed = 6.0f;
    player.yaw = 0.0f;
    player.pitch = 0.0f;

    // --- Camera setup ---
    Camera camera = { 0 };
    camera.position = player.position;
    camera.target = (Vector3){ 0.0f, 1.8f, 1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // --- Mouse look ---
        Vector2 mouseDelta = GetMouseDelta();
        const float mouseSensitivity = 0.003f;
        player.yaw -= mouseDelta.x * mouseSensitivity;
        player.pitch -= mouseDelta.y * mouseSensitivity;

        // Clamp pitch to avoid flipping
        if (player.pitch > PI / 2.0f) player.pitch = PI / 2.0f;
        if (player.pitch < -PI / 2.0f) player.pitch = -PI / 2.0f;

        // --- Direction vectors ---
        Vector3 forward = {
            sinf(player.yaw),
            0.0f,
            cosf(player.yaw)
        };

        Vector3 right = {
            cosf(player.yaw),
            0.0f,
            -sinf(player.yaw)
        };

		// --- Movement --- Vector Add/Subtract and Vector Scale from raymath.h. helps with vector math
        if (IsKeyDown(KEY_W)) player.position = Vector3Add(player.position, Vector3Scale(forward, player.speed * dt));
        if (IsKeyDown(KEY_S)) player.position = Vector3Subtract(player.position, Vector3Scale(forward, player.speed * dt));
        if (IsKeyDown(KEY_A)) player.position = Vector3Add(player.position, Vector3Scale(right, player.speed * dt));
        if (IsKeyDown(KEY_D)) player.position = Vector3Subtract(player.position, Vector3Scale(right, player.speed * dt));

		if (IsKeyDown(KEY_SPACE)) player.position.y += player.speed * dt; // Move up

        // --- Camera follows player head ---
        camera.position = player.position;
        camera.target.x = player.position.x + sinf(player.yaw) * cosf(player.pitch);
        camera.target.y = player.position.y + sinf(player.pitch);
        camera.target.z = player.position.z + cosf(player.yaw) * cosf(player.pitch);

        // --- Draw ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        // Floor
        DrawPlane((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector2) { 50.0f, 50.0f }, LIGHTGRAY);
        // Some cubes to look at
        for (int x = -20; x <= 20; x += 4)
        {
            for (int z = -20; z <= 20; z += 4)
            {
                DrawCube((Vector3) { x, 1.0f, z }, 1.0f, 2.0f, 1.0f, (Color) { 150, 100, 250, 255 });
                DrawCubeWires((Vector3) { x, 1.0f, z }, 1.0f, 2.0f, 1.0f, BLACK);
            }
        }

        EndMode3D();

        DrawText("WASD to move, mouse to look, ESC to quit", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}