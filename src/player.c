#include "player.h"

Player* player;

const float gravity = -16.0f; //gravity force
const float jumpStrength = 10.0f; // Initial jump velocity
const float groundHeight = 1.8f; // Player’s standing height from floor

void InitPlayer()
{
    player = (Player*)malloc(sizeof(Player));
    player->position = (Vector3){ 0.0f, 1.8f, 0.0f };
    player->velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
    player->speed = 8.0f;
    player->yaw = 0.0f;
    player->pitch = 0.0f;
    player->isGrounded = true;
}

Player* GetPlayer()
{
    return player;
}

void UpdatePlayer()
{
    float dt = GetFrameTime();

    Vector2 mouseDelta = GetMouseDelta();
    const float mouseSensitivity = 0.003f;
    player->yaw -= mouseDelta.x * mouseSensitivity;
    player->pitch -= mouseDelta.y * mouseSensitivity;

    // Clamp pitch to avoid flipping
    if (player->pitch > PI / 2.0f) player->pitch = PI / 2.0f;
    if (player->pitch < -PI / 2.0f) player->pitch = -PI / 2.0f;

    // Direction vectors to help with movement
    Vector3 forward = {
        sinf(player->yaw),
        0.0f,
        cosf(player->yaw)
    };

    Vector3 right = {
        cosf(player->yaw),
        0.0f,
        -sinf(player->yaw)
    };

    // if the shift key is held down, increase speed
    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        player->speed = 16.0f; // Sprint
    }
    else
    {
        player->speed = 8.0f; // Normal speed
    }

    if (IsKeyPressed(KEY_SPACE) && player->isGrounded)
    {
        player->velocity.y = jumpStrength;
        player->isGrounded = false;
    }

    // Apply the gravity if the player isn't detected on the gorund
    if (!player->isGrounded)
    {
        player->velocity.y += gravity * dt;
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
        move = Vector3Scale(move, player->speed * dt);

        Vector3 newPos = Vector3Add(player->position, (Vector3) { move.x, 0, move.z }); // only horizontal

        player->position = newPos;
    }

    // Apply vertical velocity
    player->position.y += player->velocity.y * dt;

    //simple ground collision detection
    if (player->position.y <= groundHeight)
    {
        player->position.y = groundHeight;
        player->velocity.y = 0.0f;
        player->isGrounded = true;
    }
}
 
void DestroyPlayer()
{
    RL_FREE(player);
}