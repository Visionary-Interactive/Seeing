#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "includes.h"
#include "object.h"

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Vector3 velocity;
	Vector3 lookDirection;
	Model model; // Player's 3D model
    float speed;
    float yaw;   // left/right rotation
    float pitch; // up/down rotation
	bool isGrounded; //checks if the player is on the ground
} Player;
void InitPlayer();
Player* GetPlayer();
void UpdatePlayer(GameObject* obj);
void UpdateInteractions(GameObject* obj);
BoundingBox getPlayerCollision(Model model, Vector3 position);

void checkCollision(char axis, BoundingBox _player, BoundingBox _obj);
void DestroyPlayer();

#endif