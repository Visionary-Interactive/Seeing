#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#define INVENTORY_SIZE 5
#define ANIMATION_STATES 5
#define PLAYER_FP_MODEL_PATH "resources/characters/models/player/mechFP.glb"
#define PLAYER_TP_MODEL_PATH "resources/characters/models/player/mechTP.glb"
#define MAX_PLAYERS 2

#include "includes.h"
#include "prop.h"
#include "menu.h"
#include "sessionManager.h"
#include "impairment.h"

typedef struct InputState {
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
} InputState;

typedef struct AnimationData {
    ModelAnimation* animations; // Player's animations
	int animsCount; // Number of animations
    int animFrame[ANIMATION_STATES]; // Current animation frame for each state
} AnimationData;

typedef enum {
	pickup,
	placed,
    push,
    text,
    door,
	rotate_puzzle_block,
    use_ventlid
} InteractionType;

typedef struct InventoryItem
{
    int propIndex;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    unsigned int components;
    bool occupied;
} InventoryItem;

typedef struct Checkpoint {
    bool active;
    Vector3 position;
    float yaw;
    float pitch;
} Checkpoint;

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Vector3 velocity;
	Model model; // Player's 3D model to be seen by another player
	AnimationData animData; // Player's animation data
    float speed;
    float yaw;   // left/right rotation
    float pitch; // up/down rotation
    float bottom;
	Vector3 spawnPosition;
	bool isGrounded; //checks if the player is on the ground
	bool remotePlayer; // Is this a remote player?
	InputState input; // Current input state
	InventoryItem inventory[INVENTORY_SIZE];
	int selectedSlot;
    Checkpoint checkpoint;
    int activeImpairment;
    int pendingImpairment;
	float activeImpairmentIntensity;
} Player;

extern Player* playerList[MAX_PLAYERS];

void InitPlayer();
Player* GetPlayer();
void LocalInputUpdate(struct InputState *input);
void SetPlayer(Player* p);
void UpdatePlayer(Props* obj);
void UpdateInteractions(Props* obj);

void CheckTriggers(Props* obj);
bool PlayerPropInteraction(Props* obj, InteractionType interaction, InventoryItem* slot, int propID);
BoundingBox GetPlayerCollision(Vector3 position);
bool CheckPlatformCollision(BoundingBox playerBox, float prevFeetY, BoundingBox platformBox);
void RenderPlayer(Player* p, Props* props);
void ResetPlayerToSpawn(Player* p);
void DestroyPlayer();

// Function pointer to send prop interactions to remote player through the SessionStateController
extern void (*SendPropInteractionToRemote)(InteractionType interaction, int selectedSlot, int propID);  

#endif // PLAYER_H