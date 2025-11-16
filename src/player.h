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

    // Remote movement for player
    bool ex_W;
    bool ex_A;
    bool ex_S;
    bool ex_D;
} Player;

extern const float gravity; //gravity force
extern const float jumpStrength; //initial jump velocity
extern const float groundHeight; //player’s standing height from floor

void InitPlayer();
Player* GetPlayer();
void UpdatePlayer(GameObject* obj);
void UpdateInteractions(GameObject* obj);
BoundingBox getPlayerCollision(Model model, Vector3 position);

void checkCollision(char axis, BoundingBox _player, BoundingBox _obj);
void DestroyPlayer();

#endif