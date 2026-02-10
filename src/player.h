#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#define INVENTORY_SIZE 5
#include "includes.h"
#include "prop.h"
#include "menu.h"
#include "sessionManager.h"

struct InputState {
    bool W;
    bool A;
    bool S;
    bool D;
    bool E;
    bool R;
    bool SHIFT;
    bool SPACE;
	bool ONE;
    bool TWO;
	bool THREE;
	bool FOUR;
    bool FIVE;
};

typedef struct InventoryItem
{
    int propIndex;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    unsigned int components;
    bool occupied;
} InventoryItem;

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Vector3 velocity;
	Vector3 lookDirection;
	Model model; // Player's 3D model to be seen by another player
    float speed;
    float yaw;   // left/right rotation
    float pitch; // up/down rotation
    float bottom;
	bool isGrounded; //checks if the player is on the ground
	bool remotePlayer; // Is this a remote player?
	struct InputState input; // Current input state
	InventoryItem inventory[INVENTORY_SIZE];
	int selectedSlot;

} Player;

extern bool forcePlayerTick;

void InitPlayer();
Player* GetPlayer();
void LocalInputUpdate(struct InputState *input);
void SetPlayer(Player* p);
void UpdatePlayer(Props* obj);
void UpdateInteractions(Props* obj);
BoundingBox GetPlayerCollision(float bottom, Vector3 position);


void DestroyPlayer();

#endif