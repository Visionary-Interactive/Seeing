#include "prop.h"

static Props* props;
static ParticlePool* pool;
static ParticleTemplate* template;

void CreatePropStructure(void)
{
	props = (Props*)malloc(sizeof(Props));
	memset(props, 0, sizeof(Props));
    pool = InitParticlePool(512);
	InitFlameTemplate();
	template = GetParticleTemplate();

    /*InitParticleEmitter(pool, 20.0f, (Vector3){0.0f, 0.0f, 0.0f}, template, RED);
	InitParticleEmitter(pool, 20.0f, (Vector3){10.0f, 0.0f, 0.0f}, template, WHITE);
	InitParticleEmitter(pool, 20.0f, (Vector3){20.0f, 0.0f, 0.0f}, template, YELLOW);
	InitParticleEmitter(pool, 20.0f, (Vector3){30.0f, 0.0f, 0.0f}, template, GREEN);
	InitParticleEmitter(pool, 20.0f, (Vector3){40.0f, 0.0f, 0.0f}, template, BLUE);*/
}

Props* GetPropStructure(void)
{
	return props;
}

ParticlePool* GetParticlePool(void)
{
    return pool;
}

int CreateProp(Props* obj, Model model, Vector3 position, Vector3 size, Color color, uint32_t components) {
	if (obj->count >= MAX_PROPS) {
		return -1; // Max props reached
	}
	int id = obj->count++;
	obj->position[id] = position;
	obj->size[id] = size;
	obj->model[id] = model;
	obj->color[id] = color;
	obj->rotation[id] = (Vector3){ 0.0f, 0.0f, 0.0f }; // Default rotation
    obj->interactRange[id] = (Vector3){
    obj->size[id].x + 2.0f,
    obj->size[id].y + 2.0f,
    obj->size[id].z + 2.0f
    };

	//set up collider code based on model
	ColliderSetup(obj, id);
	obj->components[id] = components; // Default components
	obj->prim[id] = NO_PRIM;
	obj->text[id] = NULL;
	obj->textType[id] = TEXTBOX_NONE;


	return id;
}

int CreatePropPrimitive(Props* obj, PrimitiveModelId prim, Vector3 position, Vector3 size, Color color, uint32_t components)
{
    Model model;
    switch (prim) {
        case PRIMITIVE_MODEL_CUBE:
            model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
            break;
        case PRIMITIVE_MODEL_DOOR:
            model = LoadModelFromMesh(GenMeshCube(3.0f, 4.0f, 0.5f));
            break;
		case PRIMITIVE_MODEL_LENS:
            model = LoadModelFromMesh(GenMeshSphere(2.0f, 32, 32));
            break;
		case PRIMITIVE_MODEL_PLATFORM:
			model = LoadModelFromMesh(GenMeshCube(4.0f, 0.5f, 4.0f));
			break;
		case PRIMITIVE_MODEL_WALL:
			model = LoadModel("resources/global/models/wall/wall.glb");
			break;
        case PRIMITIVE_MODEL_WALL2:
            model = LoadModel("resources/global/models/wall2/wall2.glb");
            break;
		case PRIMITIVE_MODEL_BUTTON:
			model = LoadModelFromMesh(GenMeshCube(1.0f, 0, 1.0f));
			break;
        case PRIMITIVE_MODEL_WARP:
            model = LoadModelFromMesh(GenMeshCube(2.0f, 0.2f, 2.0f));
            break;
        default:
            return -1;
    }
    int id = CreateProp(obj, model, position, size, color, components);
    if (id >= 0) obj->prim[id] = prim;
    return id;
}

int CreatePropFromPath(Props* obj, const char* modelPath, const char* texPath, Vector3 position, Vector3 size, Color color, uint32_t components)
{
    Model model = LoadModel(modelPath);
    int id = CreateProp(obj, model, position, size, color, components);
    Texture2D texture = LoadTexture(texPath);
    obj->model[id].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    if (id >= 0) {
        strncpy(obj->modelPath[id], modelPath, PROP_MODEL_PATH_MAX);
        strncpy(obj->texPath[id], texPath, PROP_MODEL_PATH_MAX);
        obj->modelPath[id][PROP_MODEL_PATH_MAX - 1] = '\0';
        obj->texPath[id][PROP_MODEL_PATH_MAX - 1] = '\0';
    }
    return id;
}

void CreateLight(Props* obj, int id, Color color, float intensity) {
	if (id < 0 || id >= obj->count) return;
	obj->lightColor[id] = color;
	obj->lightIntensity[id] = intensity;
	obj->components[id] |= PROP_LIGHT; // Add light component
}

void ColliderSetup(Props* obj, int id) {
	if (id < 0 || id >= obj->count) return;
	//get the boundingbox from the model, adds scale to it 
	BoundingBox bb = GetModelBoundingBox(obj->model[id]);
	Vector3 scale = obj->size[id];

	//offsets the box based on the position
	Vector3 center = Vector3Scale(Vector3Add(bb.min, bb.max), 0.5f);
    Vector3 halfExtents = Vector3Scale(Vector3Subtract(bb.max, bb.min), 0.5f);

    Vector3 scaledCenter = {
        center.x * scale.x,
        center.y * scale.y,
        center.z * scale.z
    };

    Vector3 scaledHalf = {
        fabsf(halfExtents.x * scale.x),
        fabsf(halfExtents.y * scale.y),
        fabsf(halfExtents.z * scale.z)
    };

    Vector3 worldCenter = Vector3Add(obj->position[id], scaledCenter);
    obj->collider[id].min = Vector3Subtract(worldCenter, scaledHalf);
    obj->collider[id].max = Vector3Add(worldCenter, scaledHalf);

}
//rebuild the collider for a prop based on its model and position, used for moving props and updating their colliders
BoundingBox ReBuildCollider(Props* obj, int id, Vector3 futurepos)
{

    BoundingBox bb = GetModelBoundingBox(obj->model[id]);
    Vector3 scale = obj->size[id];

    BoundingBox result;

    result.min = (Vector3){
        futurepos.x + bb.min.x * scale.x,
        futurepos.y + bb.min.y * scale.y,
        futurepos.z + bb.min.z * scale.z
    };

    result.max = (Vector3){
        futurepos.x + bb.max.x * scale.x,
        futurepos.y + bb.max.y * scale.y,
        futurepos.z + bb.max.z * scale.z
    };

    return result;
}

void AddDeadzone(Props* obj, Vector3 position, Vector3 size)
{
    int id = obj->count++;

    obj->position[id] = position;
    obj->size[id] = size;
    obj->color[id] = RED;

    obj->collider[id].min = (Vector3){
        position.x - size.x / 2,
        position.y - size.y / 2,
        position.z - size.z / 2
    };

    obj->collider[id].max = (Vector3){
        position.x + size.x / 2,
        position.y + size.y / 2,
        position.z + size.z / 2
    };

    obj->components[id] =
        PROP_VISIBILE |
        PROP_COLLIDER|
        PROP_DEADZONE;
}

int AddKillFlame(Vector3 position, Vector3 size, bool deadly)
{
    if (deadly == true) 
    {
        CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, position, size, WHITE, PROP_VISIBILE | PROP_COLLIDER | PROP_DEADZONE);
        InitParticleEmitter(pool, 20.0f, position, template, BLUE);
    }
    else InitParticleEmitter(pool, 20.0f, position, template, WHITE);
}

bool CheckCollisionWithProp(const Props* obj, int id, BoundingBox other)
{

    
    for (int i = 0; i < obj->count; i++)
    {
        if (i == id) continue;

        // Only check against objects that actually have colliders
        if (!(obj->components[i] & PROP_COLLIDER)) continue;

        if (CheckCollisionBoxes(other, obj->collider[i]))
        {
            return true;
        }
    }

    return false;
 
}

void AddPropComponent(Props* obj, int id, uint32_t componentMask)
{
    if (!obj || id < 0 || id >= (int)obj->count) return;
    obj->components[id] |= componentMask;
}

void RemovePropComponent(Props* obj, int id, uint32_t componentMask)
{
    if (!obj || id < 0 || id >= (int)obj->count) return;
    obj->components[id] &= ~componentMask;
}

void RenderProps(Props* obj) {
    for (size_t i = 0; i < obj->count; i++) {
        if ((obj->components[i] & PROP_LENS)) continue;
        if (!(obj->components[i] & PROP_VISIBILE)) continue;

		// Special rendering for deadzones for debugging sake
        if (obj->components[i] & PROP_DEADZONE)
        {
            DrawCube(
                obj->position[i],
                obj->size[i].x,
                obj->size[i].y,
                obj->size[i].z,
                RED
            );

            DrawCubeWires(
                obj->position[i],
                obj->size[i].x,
                obj->size[i].y,
                obj->size[i].z,
                BLACK
            );

            continue; // don't try to draw a model
        }

        if (obj->components[i] & PROP_DOOR)
        {

        }

            Vector3 scale = obj->size[i];
            if (scale.x == 0.0f && scale.y == 0.0f && scale.z == 0.0f) {
                scale = (Vector3){ 1.0f, 1.0f, 1.0f };
            }

            // Rotate blocks that need to be rotated. ex. if the block is not already at 90deg increment
            if (obj->interactType[i] == INTERACTABLE_PUZZLE_ROTATATION_BLOCK)
            {
                if ((int)obj->rotation[i].y % 90 != 0)
                {
                    obj->rotation[i].y += 1.0f;
                    obj->components[i] &= ~PROP_INTERACTABLE; // disable interaction while rotating
                }
                else 
                {
                    obj->components[i] |= PROP_INTERACTABLE; // re-enable interaction when done rotating
				}
            }
                

            DrawModelEx(
                obj->model[i],
                obj->position[i],
                (Vector3) {
                0.0f, 1.0f, 0.0f
            }, //rot axis
                obj->rotation[i].y, //rot angle
                scale,
                obj->color[i]
            );
    }
}

void RenderLensProps(const Props * obj)
{
    for (size_t i = 0; i < obj->count; i++) {
        if (!(obj->components[i] & PROP_LENS)) continue;
        if (!(obj->components[i] & PROP_VISIBILE)) continue;

        Vector3 scale = obj->size[i];
        if (scale.x == 0.0f && scale.y == 0.0f && scale.z == 0.0f) {
            scale = (Vector3){ 1.0f, 1.0f, 1.0f };
        }

        DrawModelEx(
            obj->model[i],
            obj->position[i],
            (Vector3) {
            0.0f, 1.0f, 0.0f
        }, //rot axis
            0.0f, //rot angle
            scale,
            obj->color[i]
        );
    }
}


void ResetProps()
{
	if (props == NULL) return;
	memset(props, 0, sizeof(Props));
}

void DestroyProps(Props* obj)
{
	RL_FREE(obj);
}