#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "includes.h"
#include "prop.h"

struct InputState {
    bool W;
    bool A;
    bool S;
    bool D;
    bool E;
    bool R;
    bool SHIFT;
    bool SPACE;
};

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
	bool remotePlayer; // Is this a remote player?
	struct InputState input; // Current input state
} Player;

extern const float gravity; //gravity force
extern const float jumpStrength; //initial jump velocity
extern const float groundHeight; //player’s standing height from floor

void InitPlayer();
Player* GetPlayer();
void LocalInputUpdate(struct InputState *input);
void SetPlayer(Player* p);
void UpdatePlayer(Props* obj);
void UpdateInteractions(Props* obj);
BoundingBox GetPlayerCollision(Model model, Vector3 position);

void CheckCollision(char axis, BoundingBox playerBox, BoundingBox propBox);
void DestroyPlayer();

#endif