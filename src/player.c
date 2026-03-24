#include "player.h"
#include "prop.h"
#include "sound.h"
#include "map.h"

Player* player;

const float gravity = -16.0f; //gravity force
const float jumpStrength = 8.0f; // Initial jump velocity
const float groundHeight = 1.8f; // Player’s standing height from floor
InventoryItem inventory[INVENTORY_SIZE] = { 0 };
int selectedSlot = 0;
Player* playerList[MAX_PLAYERS];

void (*SendPropInteractionToRemote)(InteractionType interaction, int selectedSlot, int propID) = NULL;

//initializes the player struct with default values
void InitPlayer()
{
	player = (Player*)malloc(sizeof(Player));
	player->position = (Vector3){ 0.0f, 1.8f, 47.0f };
	//player->position = (Vector3){ -12.0f, 1.8f, 5.0f };
	player->size = (Vector3){ 0.5f, 1.8f, 0.5f };
	player->velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
	player->model = LoadModel(PLAYER_FP_MODEL_PATH);
	player->animData.animations = LoadModelAnimations(PLAYER_FP_MODEL_PATH, &player->animData.animsCount);
	for (int i = 0; i < ANIMATION_STATES; i++) player->animData.animFrame[i] = 0;
	player->speed = 7.0f;
	player->yaw = PI;
	player->pitch = 0.0f;
	player->isGrounded = true;
	player->remotePlayer = false;
	player->input = (InputState){ 0 };
	player->bottom = player->position.y - player->size.y;
	player->spawnPosition = player->position;
	player->checkpoint.active = false;
	player->checkpoint.position = player->spawnPosition;
	player->checkpoint.yaw = player->yaw;
	player->checkpoint.pitch = player->pitch;
	player->activeImpairment = 0;
	player->activeImpairmentIntensity = 0.0f
;
memset(player->inventory, 0, sizeof(player->inventory));
	player->selectedSlot = 0;
}

void ManualInitPlayer(Vector3 position, Vector3 velocity, float yaw, float pitch, bool grounded)
{
    if (player == NULL)
    {
        InitPlayer();
    }

    player->position = position;
    player->velocity = velocity;
    player->yaw = yaw;
    player->pitch = pitch;
    player->isGrounded = grounded;
    player->speed = grounded ? 8.0f : player->speed; // optional default
    player->bottom = player->position.y - player->size.y;
}

static void ActivateCheckpoint(const Vector3 pos, float yaw, float pitch)
{
    player->checkpoint.active = true;
    player->checkpoint.position = pos;
    player->checkpoint.yaw = yaw;
    player->checkpoint.pitch = pitch;
}

static void RespawnAtCheckpoint(Player* p)
{
    const Checkpoint* cp = &p->checkpoint;
    if (!cp->active) return;

    p->position = cp->position;
    p->yaw = cp->yaw;
    p->pitch = cp->pitch;
    p->velocity = (Vector3){ 0 };
    p->isGrounded = true;
    p->bottom = p->position.y - p->size.y;
}

Player* GetPlayer()
{
	return player;
}

void LocalInputUpdate(InputState* input)
{
	player->input.W = IsKeyDown(KEY_W);
	player->input.A = IsKeyDown(KEY_A);
	player->input.S = IsKeyDown(KEY_S);
	player->input.D = IsKeyDown(KEY_D);
	player->input.E = IsKeyPressed(KEY_E);
	player->input.R = IsKeyDown(KEY_R);
	player->input.SHIFT = IsKeyDown(KEY_LEFT_SHIFT);
	player->input.SPACE = IsKeyPressed(KEY_SPACE);
	player->input.ONE = IsKeyPressed(KEY_ONE);
	player->input.TWO = IsKeyPressed(KEY_TWO);
	player->input.THREE = IsKeyPressed(KEY_THREE);
	player->input.FOUR = IsKeyPressed(KEY_FOUR);
	player->input.FIVE = IsKeyPressed(KEY_FIVE);

	if (player->input.ONE)   player->selectedSlot = 0;
	else if (player->input.TWO)   player->selectedSlot = 1;
	else if (player->input.THREE) player->selectedSlot = 2;
	else if (player->input.FOUR)  player->selectedSlot = 3;
	else if (player->input.FIVE)  player->selectedSlot = 4;
}

void SetPlayer(Player* p)
{
	player = p;
}

//Updates the player's position and handles input
void UpdatePlayer(Props* obj)
{
	float dt = GetFrameTime();

	Vector2 mouseDelta = GetMouseDelta();
	const float mouseSensitivity = 0.003f;
	player->yaw -= mouseDelta.x * mouseSensitivity;
	player->pitch -= mouseDelta.y * mouseSensitivity;
	player->bottom = player->position.y - player->size.y;

	const float pitchLimit = 89.0f * PI/180;
	if (player->pitch > pitchLimit) player->pitch = pitchLimit;
	if (player->pitch < -pitchLimit) player->pitch = -pitchLimit;

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

	// Set input if local player
	if (!player->remotePlayer) // Local player input
	{
		LocalInputUpdate(&player->input);
	}

	// Sprint
	if (player->input.SHIFT) player->speed = 10.0f;
	else player->speed = 7.0f; // Normal speed

	if (player->input.SPACE && player->isGrounded)
	{
		player->velocity.y = jumpStrength;
		player->isGrounded = false;
	}

	// Apply the gravity if the player isn't detected on the gorund
	if (!player->isGrounded) player->velocity.y += gravity * dt;

	if (IsTextboxStoppingPlayer())
	{
		return; // Skip movement update
	}

	// Vector Add/Subtract and Vector Scale from raymath.h. helps with vector math
	//subtract move backwards/right and add to move forwards/left
	Vector3 move = { 0 };
	if (player->input.W) move = Vector3Add(move, forward);
	if (player->input.S) move = Vector3Subtract(move, forward);
	if (player->input.A) move = Vector3Add(move, right);
	if (player->input.D) move = Vector3Subtract(move, right);

	//checks if there is any movement input
	if (Vector3Length(move) > 0.0f || player->velocity.y != 0)
	{
		move = Vector3Normalize(move);
		move.x *= player->speed; // Scale movement by speed
		move.z *= player->speed;
		move.y += player->velocity.y;
		move = Vector3Scale(move, dt); // Scale movement by delta time

		Vector3 newPos = Vector3Add(player->position, (Vector3) { move.x, move.y, move.z });
		bool blocked = false;
		BoundingBox playerBox = GetPlayerCollision(newPos); //update player bounding box

		for (size_t i = 0; i < obj->count; i++)
		{
			// Assuming PROPs is a globally accessible variable
			if (i < obj->count && (obj->components[i] & PROP_COLLIDER))
			{
				BoundingBox objBox = obj->collider[i];

				if (obj->components[i] & PROP_DEADZONE)
				{
					if (CheckCollisionBoxes(playerBox, objBox))
					{
						printf("Player entered deadzone!\n");
						ResetPlayerToSpawn(player);
						return; // stop update immediately
					}

					continue; // deadzones do not block movement
				}


				// Collision Detection
				if (CheckCollisionBoxes(playerBox, objBox))
				{

					float prevRight = player->position.x + player->size.x / 2.0f;
					float prevLeft = player->position.x - player->size.x / 2.0f;
					float prevFront = player->position.z + player->size.z / 2.0f;
					float prevBack = player->position.z - player->size.z / 2.0f;
					float prevFeetY = player->bottom;

					// Platform collision check
					if (CheckPlatformCollision(playerBox, prevFeetY, objBox))
						break;

					if (obj->components[i] & PROP_CHECKPOINT)
					{
						if (CheckCollisionBoxes(playerBox, objBox))
						{
							ActivateCheckpoint(obj->position[i], player->yaw, player->pitch);
							printf("Checkpoint reached!\n");
						}
						continue;
					}

					// Sliding along wall logic:
					if (move.x > 0 && // moving toward +X
						playerBox.max.x > objBox.min.x && // intersecting wall
						prevRight <= objBox.min.x) // was outside before
					{
						// hit right wall
						newPos.x = player->position.x; // stop horizontal movement
						continue;
					}
					else if (move.x < 0 &&
						playerBox.min.x < objBox.max.x &&
						prevLeft >= objBox.max.x)
					{
						newPos.x = player->position.x;
						continue;
					}
					else if (move.z > 0 &&
						playerBox.max.z > objBox.min.z &&
						prevFront <= objBox.min.z)
					{
						newPos.z = player->position.z;
						continue;
					}
					else if (move.z < 0 &&
						playerBox.min.z < objBox.max.z &&
						prevBack >= objBox.max.z)
					{
						newPos.z = player->position.z;
						continue;
					}

					blocked = true;
					break;
				}
				else
				{
					player->isGrounded = false; // Not colliding with platform, allow falling
				}
			}
		}

		if (!blocked)
		{
			player->position = newPos;
		}
		else // BLOCKED MOVEMENT
		{
			// Apply vertical movement if blocked
			if (!player->isGrounded)
				player->position.y += player->velocity.y * dt;
		}
	}

	//simple ground collision detection
	if (player->position.y <= groundHeight)
	{
		player->position.y = groundHeight;
		player->velocity.y = 0.0f;
		player->isGrounded = true;
	}
	CheckTriggers(obj);

}

//Grabs the player's bounding box for collision detection & makes sure it up to date on where the player is.
BoundingBox GetPlayerCollision(Vector3 position)
{
	BoundingBox playerbounding_box;
	float epsilon = 0.1f; // Small value to prevent sticking to surfaces
	Vector3 halfSize = Vector3Scale(player->size, 0.5f);

	playerbounding_box.min = (Vector3){ position.x - halfSize.x, position.y - player->size.y, position.z - halfSize.z };
	playerbounding_box.max = (Vector3){ position.x + halfSize.x , position.y + halfSize.y, position.z + halfSize.z };
	playerbounding_box.min = Vector3Subtract(playerbounding_box.min, (Vector3) { epsilon, epsilon, epsilon });
	playerbounding_box.max = Vector3Add(playerbounding_box.max, (Vector3) { epsilon, epsilon, epsilon });

	return playerbounding_box;
}

// Checks if the player is landing on a platform/cube or bonking their head on it, and updates velocity and grounded state accordingly
// Returns true if landing on platform, false if bonking head or no collision
bool CheckPlatformCollision(BoundingBox playerBox, float prevFeetY, BoundingBox platformBox)
{
	if (player->velocity.y <= 0 &&	// landing on platform
		playerBox.min.y < platformBox.max.y && // player intersects platform
		(prevFeetY + 0.1f) >= platformBox.max.y) // player was above platform last frame
	{
		player->velocity.y = 0.0f;        // stop falling
		player->isGrounded = true;     // allow jumping again
		return true;
	} 
	else if (player->velocity.y > 0 &&	// bonking head on platform from below
		playerBox.max.y > platformBox.min.y && // player intersects platform
		(prevFeetY - 0.1f) <= platformBox.min.y) // player was below platform last frame
	{
		player->velocity.y = 0.0f;        // stop going up
		return false;
	}

	return false;
}

void RenderPlayer(Player* p, Props* props)
{
	// Animations
	ModelAnimation anim;
	int animState = 0; // Default to idle
	if (!p->remotePlayer)
	{
		if (p->input.E || p->animData.animFrame[1] > 0) // animation is not finished
			animState = 1; // Interact
		else if (p->input.R || p->animData.animFrame[2] > 0)
			animState = 2; // Place
	}
	else
	{
		animState = 2; // Keep remote players in IDLE pose
	}

	animState %= p->animData.animsCount; // Ensure valid index

	if (animState != 0)
		p->animData.animFrame[0] = 0; // Reset idle animation

	anim = p->animData.animations[animState];

	p->animData.animFrame[animState] = ((p->animData.animFrame[animState] + 1) % anim.frameCount);
	UpdateModelAnimation(p->model, anim, p->animData.animFrame[animState]);

	// Draw player model with proper transformations
	if (!p->remotePlayer) // First person view for local player
	{
		Vector3 forward = {
			sinf(p->yaw) * cosf(p->pitch),
			sinf(p->pitch),
			cosf(p->yaw) * cosf(p->pitch)
		};

		Matrix rotYaw = MatrixRotateY(p->yaw + PI);
		Matrix rotPitch = MatrixRotateX(p->pitch);

		Matrix rotation = MatrixMultiply(rotPitch, rotYaw);
		Matrix scale = MatrixScale(5.0f, 5.0f, 5.0f);
		Matrix translation = MatrixTranslate(
			p->position.x,
			p->position.y,
			p->position.z
		);

		Matrix localOffset = MatrixTranslate(0.03f, -0.175f, -0.03f);

		Matrix transform = MatrixMultiply(
			localOffset,
			MatrixMultiply(scale, MatrixMultiply(rotation, translation))
		);

		// Player
		p->model.transform = transform;
		DrawModel(p->model, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
		p->model.transform = MatrixIdentity();

		// Item in hand
		InventoryItem* slot = &p->inventory[p->selectedSlot];

		if (slot->occupied)
		{
			// Spin the item in the hand
			float spinAngle = (float)GetTime() * 1.5f;
			Matrix itemSpin = MatrixRotateY(spinAngle);

			Matrix itemLocalOffset = MatrixTranslate(0.3f, 0.0f, -1.3f);
			Matrix itemScale = MatrixScale(0.5f, 0.5f, 0.5f);
			Matrix itemTransform = MatrixMultiply(
				itemSpin,
				MatrixMultiply(itemScale,
					MatrixMultiply(itemLocalOffset,
						MatrixMultiply(rotation, translation)))
			);

			props->model[slot->propIndex]->transform = itemTransform;
			DrawModel(*props->model[slot->propIndex], (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f,
				props->color[slot->propIndex]);
			props->model[slot->propIndex]->transform = MatrixIdentity();
		}
	}
	else // Remote Player
	{
		Vector3 modelPos = p->position;
		modelPos.y -= 1.9f; // Adjust model height

		DrawModelEx(p->model, modelPos, (Vector3) { 0.0f, 1.0f, 0.0f }
		, p->yaw* RAD2DEG + 180.0f, (Vector3) { 5.0f, 5.0f, 5.0f }, WHITE);
	}
}

//this sucks dude!
static int FindNearestActiveVent(Props* obj, float maxRange)
{
	int closestVent = -1;
	float closestDist = maxRange;

	for (size_t i = 0; i < obj->count; i++)
	{
		if ((obj->components[i] & PROP_VENT))
		{
			float dist = Vector3Distance(player->position, obj->position[i]);
			if (dist < closestDist)
			{
				closestDist = dist;
				closestVent = (int)i;
			}
		}
	}

	return closestVent;
}

//checks for interactions between all objects in the scene and the player
void UpdateInteractions(Props* obj)
{
	int closestID = -1;
	float closestDist = 99999.0f;

	Vector3 p = player->position;

	for (size_t i = 0; i < obj->count; i++)
	{
		if (!(obj->components[i] & PROP_VISIBILE &&
			obj->components[i] & PROP_INTERACTABLE))
			continue;

		Vector3 o = obj->position[i];
		float dist = Vector3Distance(p, o);
		float range = obj->interactRange[i].x;

		if (dist < range && dist < closestDist)
		{
			closestDist = dist;
			closestID = i;
		}
	}

	if (closestID != -1)
	{
		if (!player->remotePlayer)
			DrawText("Press E to interact:", 10, 200, 30, BLACK);

		if (player->input.E)
		{
			switch (obj->interactType[closestID])
			{
			case INTERACTABLE_DOOR:
				PlayerPropInteraction(obj, door, NULL, closestID);
				break;

			case INTERACTABLE_PICKUP:
			{
				InventoryItem* slot =
					&player->inventory[player->selectedSlot];

				if (!slot->occupied)
				{
					PlaySound(pickupItem1);
					PlayerPropInteraction(obj, pickup, slot, closestID);
					SendPropInteractionToRemote(pickup,
						player->selectedSlot,
						closestID);
				}
				break;
			}

			//vent lid pickup
			case INTERACTABLE_VENTLID:
			{
				InventoryItem* slot =
					&player->inventory[player->selectedSlot];

				if (!slot->occupied)
				{
					PlaySound(pickupItem1);
					PlayerPropInteraction(obj, pickup, slot, closestID);
					SendPropInteractionToRemote(pickup,
						player->selectedSlot,
						closestID);
				}
				break;
			}

			case INTERACTABLE_TEXT:
				PlayerPropInteraction(obj, text, NULL, closestID);
				break;

			case INTERACTABLE_PUSH:
				if (!player->remotePlayer)
				{
					if (PlayerPropInteraction(obj, push, NULL, closestID))
						SendPropInteractionToRemote(push,
							-1,
							closestID);
				}
				break;

			case INTERACTABLE_PUZZLE_ROTATATION_BLOCK:
				if (!player->remotePlayer)
				{
					PlayerPropInteraction(obj, rotate_puzzle_block, NULL, closestID);
					SendPropInteractionToRemote(rotate_puzzle_block, -1, closestID);
				}
				break;
			}
		}
	}

	if (player->input.R)
	{
		InventoryItem* slot = &player->inventory[player->selectedSlot];

		if (slot->occupied)
		{
			if (slot->components & PROP_VENTLID)
			{
				float ventUseRange = 4.0f;
				int ventID = FindNearestActiveVent(obj, ventUseRange);

				if (ventID != -1)
				{
					PlaySound(placeItem1);
					PlayerPropInteraction(obj, use_ventlid, slot, ventID);
					SendPropInteractionToRemote(use_ventlid,
						player->selectedSlot, ventID);
				}
				else
				{
					PlaySound(placeItem1);
					PlayerPropInteraction(obj, placed, slot, slot->propIndex);
					SendPropInteractionToRemote(placed,
						player->selectedSlot, slot->propIndex);
				}
			}
			else
			{
				PlaySound(placeItem1);
				PlayerPropInteraction(obj, placed, slot, slot->propIndex);
				SendPropInteractionToRemote(placed,
					player->selectedSlot, slot->propIndex);
			}
		}
	}
}

Vector3 GetCardinalDirection(Vector3 forward)
{
	Vector3 dir = { 0 };

	if (fabsf(forward.x) > fabsf(forward.z))
	{
		dir.x = (forward.x > 0) ? 1 : -1;
	}
	else
	{
		dir.z = (forward.z > 0) ? 1 : -1;
	}

	return dir;
}

void CheckTriggers(Props* obj)
{
	BoundingBox playerBox = GetPlayerCollision(player->position); //update player bounding box
	for (size_t i = 0; i < obj->count; i++)
	{
		if (!(obj->components[i] & PROP_TRIGGERZONE)) continue;

		BoundingBox objBox = obj->collider[i];

		if (CheckCollisionBoxes(playerBox, objBox))
		{
			//printf("Player entered Triggerzone!\n");

			// Example behaviors
			if (obj->triggerType[i] == TRIGGER_WARP)
			{
				player->position = obj->warpTarget[i]; // Warp player to prop position
				player->velocity = (Vector3){ 0 }; // Stop any existing velocity
				player->spawnPosition = obj->warpTarget[i]; // Update spawn position to new location
			}
			else if (obj->triggerType[i] == TRIGGER_DEADZONE)
			{
				ResetPlayerToSpawn(player);
				printf("Entered Deadzone Trigger! by %s", obj[i]);
			}
			else if (obj->triggerType[i] == TRIGGER_CHECKPOINT)
			{
				ActivateCheckpoint(obj->position[i], player->yaw, player->pitch);
				printf("Checkpoint Trigger\n");
			}
			else if (obj->triggerType[i] == TRIGGER_TEXT && obj->Triggered[i] == false)
			{
				if (!player->remotePlayer) // Only trigger text for local player
				{
					InitTextBox(obj->textType[i], obj->text[i]);
					obj->Triggered[i] = true; // Prevent retriggering if desired
					//printf("Triggered text box: %s\n", obj->text[i]);
				}
			}
			else if (obj->triggerType[i] == TRIGGER_IMPAIRMENT)
			{
				//printf("Triggered impairment: %d\n", obj->ImpairmentType[i]);
				//obj->Triggered[i] = true;
				player->pendingImpairment = obj->ImpairmentType[i];
				player->pendingIntensity = 0.9f; // Example intensity
			}

		}
	}
}


bool PlayerPropInteraction(Props* obj, InteractionType interaction, InventoryItem* slot, int propID)
{
	if (interaction == pickup)
	{
		slot->propIndex = propID;
		slot->position = obj->position[propID];
		slot->components = obj->components[propID];
		slot->occupied = true;

		obj->components[propID] &= ~PROP_VISIBILE;
		obj->components[propID] &= ~PROP_COLLIDER;

		printf("Picked up prop %d into slot %d\n", propID, player->selectedSlot);
	}
	if (interaction == door)
	{
		RequestExit();

	}
	else if (interaction == text)
	{
		InitTextBox(obj->textType[propID], obj->text[propID]);
	}
	else if (interaction == placed)
	{
		Vector3 dir = {
			sinf(player->yaw) * cosf(player->pitch),
			sinf(player->pitch),
			cosf(player->yaw) * cosf(player->pitch)
		};

		Vector3 placePos = Vector3Add(
			player->position,
			Vector3Scale(Vector3Normalize(dir), 2.0f)
		);

		obj->position[propID] = placePos;
		ColliderSetup(obj, propID);
		obj->components[propID] = slot->components | PROP_VISIBILE | PROP_COLLIDER;

		slot->occupied = false;

		printf("Placed prop %d from slot %d\n", propID, player->selectedSlot);
	}
	else if (interaction == push)
	{
		player->velocity.y += 0.001f; // Small movement to trigger collision check
		printf("Pushing prop %d\n", propID);
		Vector3 playerForward = (Vector3){
			sinf(player->yaw),
			0.0f,
			cosf(player->yaw)
		};
		int pushDistance = 2.0f;
		if (!(obj->components[propID] & PROP_PUSHABLE)) return false;

		Vector3 moveDir = GetCardinalDirection(playerForward);
		Vector3 moveAmount = Vector3Scale(moveDir, pushDistance);
		Vector3 futurePosition = Vector3Add(obj->position[propID], moveAmount);
		BoundingBox futureBox = ReBuildCollider(obj, propID, futurePosition);

		if (CheckCollisionWithProp(obj, propID, futureBox))
		{
			printf("Cannot push prop %d, path is blocked!\n", propID);
			return false;
		}

		if (playerList[1] != NULL && CheckCollisionBoxes(GetPlayerCollision(playerList[1]->position), futureBox))
		{
			printf("Cannot push prop %d, player is in the way!\n", propID);
			return false;
		}
		Vector3 newPos = Vector3Add(obj->position[propID], moveAmount);
		if (!player->remotePlayer)
			PlaySound(pushBoulder);
		// Notify prop to move towards new position over time in the render update
		snprintf(obj->text[propID], 255, "%d,%d",
						(int)newPos.x,
						(int)newPos.z);
	}
	else if (interaction == rotate_puzzle_block)
	{
		obj->rotation[propID].y += 5.0f;
		if (!player->remotePlayer)
			PlaySound(rotatingPuzzleBlock);
		int blockNum = atoi(obj->text[propID]);
		blockNum = (blockNum + 1) % 4;

		char buf[2];
		sprintf(buf, "%d", blockNum);
		obj->text[propID] = strdup(buf);
		printf("Rotated puzzle block %d to orientation %d\n", propID, blockNum);
	}
	//use vent lid on a fire vent
	else if (interaction == use_ventlid)
	{
		// propID here is the fire vent index
		// Deactivate the fire vent
		obj->components[propID] &= ~PROP_DEADZONE;
		obj->components[propID] &= ~PROP_VENT;
		obj->components[propID] &= ~PROP_TRIGGERZONE;

		// Snap the lid visually on top of the vent
		int lidPropID = slot->propIndex;
		obj->position[lidPropID] = obj->position[propID];
		obj->position[lidPropID].y = obj->position[propID].y +
			(obj->collider[propID].max.y - obj->collider[propID].min.y) * 0.5f;

		// Make the lid visible again (placed on the vent) but no longer interactable
		obj->components[lidPropID] |= PROP_VISIBILE;
		obj->components[lidPropID] &= ~PROP_INTERACTABLE;
		obj->components[lidPropID] &= ~PROP_VENTLID;
		ColliderSetup(obj, lidPropID);

		slot->occupied = false;

		printf("Used vent lid (prop %d) to disable fire vent (prop %d)\n",
			lidPropID, propID);
	}

	return true;
}

void ResetPlayerToSpawn(Player* p)
{
	player->position = player->spawnPosition;

	// Rebuild collider
	GetPlayerCollision(player->position);

	player->velocity = (Vector3){ 0 };
}

void DestroyPlayer()
{
	RL_FREE(player);
}