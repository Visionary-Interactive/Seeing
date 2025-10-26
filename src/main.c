#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"
#include <math.h>

typedef struct Player {
    Vector3 position;
    Vector3 velocity;
	Vector3 lookDirection;
    float speed;
    float yaw;   // left/right rotation
    float pitch; // up/down rotation
	bool isGrounded; //checks if the player is on the ground
} Player;


static void DrawLevel(void);


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "A Game about Seeing");
    DisableCursor();   // Hide cursor for mouselook
    SetTargetFPS(60);

    //Sets up the player sturct
    Player player = { 0 };
    player.position = (Vector3){ 0.0f, 1.8f, 0.0f };
    player.speed = 8.0f;
    player.yaw = 0.0f;
    player.pitch = 0.0f;
	player.isGrounded = true;

    //Sets up camera
    Camera camera = { 0 };
    camera.position = player.position;
    camera.target = (Vector3){ 0.0f, 1.8f, 1.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const float gravity = -16.0f; //gravity force
	const float jumpStrength = 10.0f; // Initial jump velocity
    const float groundHeight = 1.8f; // Player’s standing height from floor

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        //getting the direction the mosue is moving for the purposes of the camera
        Vector2 mouseDelta = GetMouseDelta();
        const float mouseSensitivity = 0.003f;
        player.yaw -= mouseDelta.x * mouseSensitivity;
        player.pitch -= mouseDelta.y * mouseSensitivity;

        // Clamp pitch to avoid flipping
        if (player.pitch > PI / 2.0f) player.pitch = PI / 2.0f;
        if (player.pitch < -PI / 2.0f) player.pitch = -PI / 2.0f;

		// Direction vectors to help with movement
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

		// if the shift key is held down, increase speed
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            player.speed = 16.0f; // Sprint
        }
        else
        {
            player.speed = 8.0f; // Normal speed
		}

        if (IsKeyPressed(KEY_SPACE) && player.isGrounded)
        {
            player.velocity.y = jumpStrength;
            player.isGrounded = false;
        }

        // Apply the gravity if the player isn't detected on the gorund
        if (!player.isGrounded)
        {
            player.velocity.y += gravity * dt;
        }



        
        // Vector Add/Subtract and Vector Scale from raymath.h. helps with vector math
		//subtract move backwards/right and add to move forwards/left
        //
        Vector3 move = { 0 };
        if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
        if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
        if (IsKeyDown(KEY_A)) move = Vector3Add(move, right);
        if (IsKeyDown(KEY_D)) move = Vector3Subtract(move, right);

		//checks if there is any movement input
        if (Vector3Length(move) > 0.0f)
        {
            move = Vector3Normalize(move);
            move = Vector3Scale(move, player.speed * dt);

            Vector3 newPos = Vector3Add(player.position, (Vector3) { move.x, 0, move.z }); // only horizontal

            player.position = newPos;
        }

		// Apply vertical velocity
        player.position.y += player.velocity.y * dt;

		//simple ground collision detection
        if (player.position.y <= groundHeight)
        {
            player.position.y = groundHeight;
            player.velocity.y = 0.0f;
            player.isGrounded = true;
        }


        //allows the camera to follow the player's head based on rotation
        camera.position = player.position;
        camera.target.x = player.position.x + sinf(player.yaw) * cosf(player.pitch);
        camera.target.y = player.position.y + sinf(player.pitch);
        camera.target.z = player.position.z + cosf(player.yaw) * cosf(player.pitch);

		// starts to Draw the 3D world
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawLevel();

        EndMode3D();
		//creates the text on the screen
        DrawText("WASD to move, Mouse to look, ,ESC to quit", 10, 10, 15, DARKGRAY);
		DrawText("Shift to sprint, Space to jump", 10, 25, 15, DARKGRAY);

        EndDrawing();
    }

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
            if ((y & 1) && (x & 1))
            {
                DrawPlane((Vector3) { x* tileSize, 0.0f, y* tileSize }, (Vector2) { tileSize, tileSize }, tileColor1);
            }
            else if (!(y & 1) && !(x & 1))
            {
                DrawPlane((Vector3) { x* tileSize, 0.0f, y* tileSize }, (Vector2) { tileSize, tileSize }, PURPLE);
            }
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

