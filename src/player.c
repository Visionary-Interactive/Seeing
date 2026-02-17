#include "player.h"
#include "prop.h"
#include "sound.h"

Player* player;

const float gravity = -16.0f; //gravity force
const float jumpStrength = 10.0f; // Initial jump velocity
const float groundHeight = 1.8f; // Player’s standing height from floor
InventoryItem inventory[INVENTORY_SIZE] = { 0 };
int selectedSlot = 0;

void (*SendPropInteractionToRemote)(InteractionType interaction, int selectedSlot, int propID) = NULL;

//initializes the player struct with default values
void InitPlayer()
{
	player = (Player*)malloc(sizeof(Player));
	player->position = (Vector3){ 0.0f, 1.8f, 0.0f };
	player->size = (Vector3){ 0.5f, 1.8f, 0.5f };
	player->velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
	player->model = LoadModelFromMesh(GenMeshCube(0.5f, 1.8f, 0.5f));
	player->speed = 8.0f;
	player->yaw = 0.0f;
	player->pitch = 0.0f;
	player->isGrounded = true;
	player->remotePlayer = false;
	player->input = (struct InputState){ 0 };
	player->bottom = player->position.y - player->size.y;
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

Player* GetPlayer()
{
	return player;
}

void LocalInputUpdate(struct InputState* input)
{
	player->input.W = IsKeyDown(KEY_W);
	player->input.A = IsKeyDown(KEY_A);
	player->input.S = IsKeyDown(KEY_S);
	player->input.D = IsKeyDown(KEY_D);
	player->input.E = IsKeyDown(KEY_E);
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
	if (player->input.SHIFT) player->speed = 16.0f;
	else player->speed = 8.0f; // Normal speed

	if (player->input.SPACE && player->isGrounded)
	{
		player->velocity.y = jumpStrength;
		player->isGrounded = false;
	}

	// Apply the gravity if the player isn't detected on the gorund
	if (!player->isGrounded) player->velocity.y += gravity * dt;

	// Vector Add/Subtract and Vector Scale from raymath.h. helps with vector math
	//subtract move backwards/right and add to move forwards/left
	//
	Vector3 move = { 0 };
	if (player->input.W) move = Vector3Add(move, forward);
	if (player->input.S) move = Vector3Subtract(move, forward);
	if (player->input.A) move = Vector3Add(move, right);
	if (player->input.D) move = Vector3Subtract(move, right);

	//checks if there is any movement input
	if (Vector3Length(move) > 0.0f || player->velocity.y != 0)
	{
		float prevFeetY = player->bottom;
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

				if (CheckCollisionBoxes(playerBox, objBox)) 
				{
					// Platform collision check
					if (obj->prim[i] == PRIMITIVE_MODEL_PLATFORM || obj->prim[i] == PRIMITIVE_MODEL_CUBE)
					{
						if (CheckPlatformCollision(playerBox, prevFeetY, objBox))
							break;
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
		else
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
		(prevFeetY + 0.5f) >= platformBox.max.y) // player was above platform last frame
	{
		player->velocity.y = 0.0f;        // stop falling
		player->isGrounded = true;     // allow jumping again
		return true;
	} 
	else if (player->velocity.y > 0 &&	// bonking head on platform from below
		playerBox.max.y > platformBox.min.y && // player intersects platform
		(prevFeetY - 0.5f) <= platformBox.min.y) // player was below platform last frame
	{
		player->velocity.y = 0.0f;        // stop going up
		return false;
	}

	return false;
}

void UpdateInteractions(Props* obj)
{
	for (size_t i = 0; i < obj->count; i++)
	{
		if (!(obj->components[i] & PROP_VISIBILE && obj->components[i] & PROP_INTERACTABLE)) continue;

		Vector3 p = player->position;
		Vector3 o = obj->position[i];

		float dist = Vector3Distance(p, o);
		float range = obj->interactRange[i].x; // use X as range radius

		if (dist < range)
		{
			// Show prompt
			if (!player->remotePlayer)
				DrawText("Press E to interact:", 10, 200, 30, BLACK);

			if (player->input.E)
			{
				switch (obj->interactType[i])
				{
				case INTERACTABLE_DOOR:
					if (!player->remotePlayer)
					{
						DrawText("You interacted with a door!", 10, 200, 24, GREEN);
						RequestExit();
					}
					break;
				case INTERACTABLE_PICKUP:
				{
					InventoryItem* slot = &player->inventory[player->selectedSlot];
					if (!slot->occupied && !player->remotePlayer)
					{
						PlayerPropInteraction(obj, pickup, slot, i);
						SendPropInteractionToRemote(pickup, player->selectedSlot, i);
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}
	//allows the player to replace the prop based on where they are looking
	if (player->input.R)
	{
		InventoryItem* slot = &player->inventory[player->selectedSlot];

		if (slot->occupied)
		{
			PlayerPropInteraction(obj, placed, slot, slot->propIndex);
			SendPropInteractionToRemote(placed, player->selectedSlot, slot->propIndex);
		}
	}
}

void PlayerPropInteraction(Props* obj, InteractionType interaction, InventoryItem* slot, int propID)
{
	if (interaction == pickup)
	{
		PlaySound(pickupItem1);
		slot->propIndex = propID;
		slot->position = obj->position[propID];
		slot->components = obj->components[propID];
		slot->occupied = true;


		obj->components[propID] &= ~PROP_VISIBILE;
		obj->components[propID] &= ~PROP_COLLIDER;

		printf("Picked up prop %d into slot %d\n", propID, player->selectedSlot);
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
		obj->collider[propID] = ReBuildCollider(obj->model[propID], placePos);
		obj->components[propID] = slot->components | PROP_VISIBILE | PROP_COLLIDER;

		slot->occupied = false;

		printf("Placed prop %d from slot %d\n", propID, player->selectedSlot);
	}
}

void DestroyPlayer()
{
	RL_FREE(player);
}