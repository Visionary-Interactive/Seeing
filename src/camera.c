#include "camera.h"

Camera* cam;
int cameraMode;

void InitCamera()
{
    cam = (Camera*)malloc(sizeof(Camera));
    cam->position = (Vector3){ 0.0f, 1.8f, 0.0f };
    cam->target = (Vector3){ 0.0f, 1.8f, 1.0f };
    cam->up = (Vector3){ 0.0f, 1.0f, 0.0f };
    cam->fovy = 90.0f;
    cam->projection = CAMERA_PERSPECTIVE;
    cameraMode = CAMERA_FIRST_PERSON;
}

Camera* GetCamera()
{
    return cam;
}

void RefreshCamera(Player* player)
{
    //allows the camera to follow the player's head based on rotation
    cam->position = player->position;
    cam->target.x = player->position.x + sinf(player->yaw) * cosf(player->pitch);
    cam->target.y = player->position.y + sinf(player->pitch);
    cam->target.z = player->position.z + cosf(player->yaw) * cosf(player->pitch);
}

void DestroyCamera()
{
    RL_FREE(cam);
}