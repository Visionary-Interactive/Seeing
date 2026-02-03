#include "player.h"
#include "prop.h"

Player* player;

const float gravity = -16.0f; //gravity force
int heldObject = -1; // No object held initially
const float jumpStrength = 10.0f; // Initial jump velocity
const float groundHeight = 1.8f; // Player’s standing height from floor

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
	if (Vector3Length(move) > 0.0f)
	{
		float prevFeetY = player->bottom;
		move = Vector3Normalize(move);
		move = Vector3Scale(move, player->speed * dt);

		Vector3 newPos = Vector3Add(player->position, (Vector3) { move.x, 0, move.z }); // only horizontal
		GetPlayerCollision(player->bottom, newPos); //update player bounding box
		bool blocked = false;
		BoundingBox playerBox = GetPlayerCollision(player->bottom, newPos);
		for (size_t i = 0; i < obj->count; i++) {
			// Assuming PROPs is a globally accessible variable
			if (i < obj->count && (obj->components[i] & PROP_COLLIDER)) {
				BoundingBox objBox = obj->collider[i];

				if (CheckCollisionBoxes(playerBox, objBox)) {
					blocked = true;
					break;
				}
			}
		}
		if (!blocked)
		{
			player->position = newPos;
		}

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

//Grabs the player's bounding box for collision detection & makes sure it up to date on where the player is.
BoundingBox GetPlayerCollision(float bottom, Vector3 position)
{

	BoundingBox playerbounding_box;

	playerbounding_box.min = (Vector3){ position.x - player->size.x, position.y - player->size.y, position.z - player->size.z };
	playerbounding_box.max = (Vector3){ position.x + player->size.x , position.y + bottom, position.z + player->size.z };
	return playerbounding_box;
}


void CheckGroundCollision(BoundingBox playerBox, BoundingBox platformBox)
{
	if (CheckCollisionBoxes(playerBox, platformBox))
	{
		// Simple ground collision response
		if (player->position.y > platformBox.max.y)
		{
			player->position.y = platformBox.max.y + player->size.y / 2.0f;
			player->velocity.y = 0;        // stop falling
			player->isGrounded = true;     // allow jumping again
		}
	}
}

void UpdateInteractions(Props* obj)
{
	// Set input if local player
	if (!player->remotePlayer) // Local player input
	{
		LocalInputUpdate(&player->input);
	}

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
				DrawText("Press E to interact:", 10, 200, 24, RED);

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
					if (heldObject == -1)
					{
						heldObject = i;

						// Hide object in world
						obj->components[i] &= ~PROP_VISIBILE;
						obj->components[i] &= ~PROP_COLLIDER;

                            printf("Picked up object %zu\n", i);
                            SetUIInteraction(true);

                        }
                        break;
                    default:
                        DrawText("You interacted with something!", 10, 200, 24, GREEN);
                        break;
				}
			}
			if (player->input.R)
			{
				//get the direction the player is facing
				Vector3 dir = {
				sinf(player->yaw) * cosf(player->pitch),
				sinf(player->pitch),
				cosf(player->yaw) * cosf(player->pitch)
				};

				//simple position in front of player
				Vector3 placePos = Vector3Add(player->position, Vector3Scale(dir, 2.0f));

				obj->position[heldObject] = placePos;

				// Show object again
				obj->components[heldObject] |= PROP_VISIBILE;
				obj->components[heldObject] |= PROP_COLLIDER;

				printf("Placed object %d\n", heldObject);
				DrawText("Placed object", 10, 120, 24, GREEN);

				heldObject = -1;
			}

		}
	}
}

void DestroyPlayer()
{
	RL_FREE(player);
}