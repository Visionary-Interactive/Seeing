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
}

Props* GetPropStructure(void)
{
	return props;
}

ParticlePool* GetParticlePool(void)
{
    return pool;
}

int CreateProp(Props* obj, Model* model, Vector3 position, Vector3 size, Color color, uint32_t components) {
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

	//set up text to null by default
	obj->text[id] = NULL;
	obj->textType[id] = TEXTBOX_NONE;

	obj->triggerType[id] = TRIGGER_NONE;
	obj->ImpairmentType[id] = 0;

	obj->warpTarget[id] = (Vector3){ 0.0f, 0.0f, 0.0f };

	return id;
}

int CreatePropPrimitive(Props* obj, PrimitiveModelId prim, Vector3 position, Vector3 size, Color color, uint32_t components)
{
    Model* model = GetCachedPrimitive(prim);
    if (!model) return -1;

    int id = CreateProp(obj, model, position, size, color, components);
    if (id >= 0) obj->prim[id] = prim;

    return id;
}

int CreatePropFromPath(Props* obj, const char* modelPath, const char* texPath, Vector3 position, Vector3 size, Color color, uint32_t components)
{
    Model* model = GetCachedModel(modelPath);
    if (!model) return -1;

    int id = CreateProp(obj, model, position, size, color, components);
    Texture2D* tex = GetCachedTexture(texPath);
    obj->model[id]->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *tex;
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
	BoundingBox bb = GetModelBoundingBox(*obj->model[id]);
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

    BoundingBox bb = GetModelBoundingBox(*obj->model[id]);
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

int AddZone(Props* obj, Vector3 position, Vector3 size, TriggerType type)
{
    int id = CreatePropPrimitive(obj, PRIMITIVE_MODEL_CUBE, position, size, BLANK, PROP_TRIGGERZONE | PROP_VISIBILE);

    obj->triggerType[id] = type;
    obj->Triggered[id] = false;

    return id;
}


int AddKillFlame(Vector3 position, Vector3 size, bool deadly)
{
    if (deadly == true) 
    {
        InitParticleEmitter(pool, 20.0f, position, template, BLUE);
        int flame = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, position, size, WHITE, 
            PROP_VISIBILE | PROP_TRIGGERZONE | PROP_VENT);
		props->triggerType[flame] = TRIGGER_DEADZONE;
		return flame;
    }
    else {
        InitParticleEmitter(pool, 20.0f, position, template, WHITE);
        return -1;
    }
}


void AddWarpZone(Vector3 position, Vector3 size, Vector3 Warpposition)
{
	CreatePropPrimitive(props, PRIMITIVE_MODEL_WARP, position, size, PURPLE, PROP_VISIBILE);
    int id = CreatePropPrimitive(props, PRIMITIVE_MODEL_CUBE, position, size, RED, PROP_VISIBILE | PROP_COLLIDER);

	//AddPropComponent(props, id, PROP_WARP);
	
	//props->warpPosition[id] = Warpposition;

    
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

        if ((obj->components[i] & PROP_TRIGGERZONE) && !obj->Triggered[i])
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

        if (obj->triggerType[i] == TRIGGER_TEXT)
        {
            DrawCube(
                obj->position[i],
                obj->size[i].x,
                obj->size[i].y,
                obj->size[i].z,
                GREEN
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

        if (obj->triggerType[i] == TRIGGER_IMPAIRMENT)
        {
            DrawCube(
                obj->position[i],
                obj->size[i].x,
                obj->size[i].y,
                obj->size[i].z,
                PURPLE
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
                *obj->model[i],
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
            *obj->model[i],
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