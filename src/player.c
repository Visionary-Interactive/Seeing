#include "player.h"
#include "object.h"

Player* player;

const float gravity = -16.0f; //gravity force
int heldObject = -1; // No object held initially
const float jumpStrength = 10.0f; // Initial jump velocity
const float groundHeight = 1.8f; // Player’s standing height from floor

void InitPlayer()
{
    player = (Player*)malloc(sizeof(Player));
    player->position = (Vector3){ 0.0f, 1.8f, 0.0f };
	player->size = (Vector3){ 0.5f, 1.8f, 0.5f };
    player->velocity = (Vector3){ 0.0f, 0.0f, 0.0f };
    //morgan - we should save this for the networking logic. the client does not need to load a model it will never see for itself
	player->model = LoadModelFromMesh(GenMeshCube(0.5f, 1.8f, 0.5f)); // Simple cube as player model
    player->speed = 8.0f;
    player->yaw = 0.0f;
    player->pitch = 0.0f;
    player->isGrounded = true;

	// Remote movement initialization
    player->ex_W = false;
    player->ex_A = false;
    player->ex_S = false;
    player->ex_D = false;
}

Player* GetPlayer()
{
    return player;
}

void UpdatePlayer(GameObject* obj)
{
    float dt = GetFrameTime();

    Vector2 mouseDelta = GetMouseDelta();
    const float mouseSensitivity = 0.003f;
    player->yaw -= mouseDelta.x * mouseSensitivity;
    player->pitch -= mouseDelta.y * mouseSensitivity;

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

    // if the shift key is held down, increase speed
    if (IsKeyDown(KEY_LEFT_SHIFT)) player->speed = 16.0f; // Sprint
    else player->speed = 8.0f; // Normal speed

    if (IsKeyPressed(KEY_SPACE) && player->isGrounded)
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
    if (IsKeyDown(KEY_W) || player->ex_W) move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S) || player->ex_S) move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_A) || player->ex_A) move = Vector3Add(move, right);
    if (IsKeyDown(KEY_D) || player->ex_D) move = Vector3Subtract(move, right);

    //checks if there is any movement input
    if (Vector3Length(move) > 0.0f)
    {

        move = Vector3Normalize(move);
        move = Vector3Scale(move, player->speed * dt);

        Vector3 newPos = Vector3Add(player->position, (Vector3) { move.x, 0, move.z }); // only horizontal
		getPlayerCollision(player->model, newPos); //update player bounding box
        bool blocked = false;

        for (size_t i = 0; i < obj->count; i++) {
            // Assuming gameObjects is a globally accessible variable
            if (i < obj->count && (obj->components[i] & OBJECT_COLLIDER)) {
                BoundingBox objBox = obj->collider[i];
                BoundingBox playerBox = getPlayerCollision(player->model, newPos);
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
BoundingBox getPlayerCollision(Model model, Vector3 position)
{
	BoundingBox playerbounding_box = GetMeshBoundingBox(model.meshes[0]);
	Vector3 min_boundary = Vector3Add(position, playerbounding_box.min);
	Vector3 max_boundary = Vector3Add(position, playerbounding_box.max);
    playerbounding_box.max = max_boundary;
	playerbounding_box.min = min_boundary;
    return playerbounding_box;
}

void checkCollision(char axis, BoundingBox _player, BoundingBox _obj)
{
    if (CheckCollisionBoxes(_player, _obj))
    {
        if (axis == 'x')
        {
            if (player->position.x < _obj.min.x)
            {
                player->position.x = _obj.min.x - player->size.x / 2.0f;
            }
            else if (player->position.x > _obj.max.x)
            {
                player->position.x = _obj.max.x + player->size.x / 2.0f;
            }
        }
        else if (axis == 'z')
        {
            if (player->position.z < _obj.min.z)
            {
                player->position.z = _obj.min.z - player->size.z / 2.0f;
            }
            else if (player->position.z > _obj.max.z)
            {
                player->position.z = _obj.max.z + player->size.z / 2.0f;
            }
        }
        else if (axis == 'y')
        {
            if (player->position.y < _obj.min.y)
            {
                // Player is hitting underside of the object (ceiling)
                player->position.y = _obj.min.y - player->size.y / 2.0f;
                player->velocity.y = 0;        // stop upwards velocity
            }
            else if (player->position.y > _obj.max.y)
            {
                // Player lands on top of the object
                player->position.y = _obj.max.y + player->size.y / 2.0f;
                player->velocity.y = 0;        // stop falling
                player->isGrounded = true;     // allow jumping again
            }
        }
	}

}

void UpdateInteractions(GameObject* obj)
{
    for (size_t i = 0; i < obj->count; i++)
    {
        if (obj->components[i] & OBJECT_INTERACTABLE) continue;

        Vector3 p = player->position;
        Vector3 o = obj->position[i];

        float dist = Vector3Distance(p, o);
        float range = obj->InteractRange[i].x; // use X as range radius

        if (dist < range)
        {
            // Show prompt
            DrawText("Press E to interact:", 10, 90, 24, YELLOW);

            if (IsKeyPressed(KEY_E))
            {
                switch (obj->interactType[i])
                {
                    case INTERACTABLE_DOOR:
                        DrawText("You interacted with a door!", 10, 120, 24, GREEN);
                        CloseWindow();
                        break;
                    case INTERACTABLE_PICKUP:
                        if (heldObject == -1)
                        {
                            heldObject = i;

                            // Hide object in world
                            obj->components[i] &= ~OBJECT_VISIBILE;
                            obj->components[i] &= ~OBJECT_COLLIDER;

                            printf("Picked up object %zu\n", i);
                        }
                        break;
                    default:
                        DrawText("You interacted with something!", 10, 120, 24, GREEN);
                        break;
				}
            }
            if (IsKeyPressed(KEY_R))
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
                obj->components[heldObject] |= OBJECT_VISIBILE;
                obj->components[heldObject] |= OBJECT_COLLIDER;

                printf("Placed object %d\n", heldObject);
                DrawText("Palced object", 10, 120, 24, GREEN);

                heldObject = -1;
            }
           
        }
    }
}

void DestroyPlayer()
{
    RL_FREE(player);
}