#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "includes.h"

typedef struct Player {
    Vector3 position;
    Vector3 velocity;
	Vector3 lookDirection;
    float speed;
    float yaw;   // left/right rotation
    float pitch; // up/down rotation
	bool isGrounded; //checks if the player is on the ground
} Player;

extern const float gravity; //gravity force
extern const float jumpStrength; //initial jump velocity
extern const float groundHeight; //player’s standing height from floor

void InitPlayer();
Player* GetPlayer();
void UpdatePlayer();
void DestroyPlayer();

#endif