#include "prop.h"

static Props* props;

void CreatePropStructure(void)
{
	props = (Props*)malloc(sizeof(Props));
	memset(props, 0, sizeof(Props));
}

Props* GetPropStructure(void)
{
	return props;
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
	obj->interactRange[id] = (Vector3){ 2.0f,2.0f,2.0f }; // Default interaction range

	//set up collider code based on model
	ColliderSetup(obj, id);
	obj->components[id] = components; // Default components
	obj->prim[id] = NO_PRIM;

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
			model = LoadModelFromMesh(GenMeshCube(0.5f, 4.0f, 4.0f));
			break;
		case PRIMITIVE_MODEL_BUTTON:
			model = LoadModelFromMesh(GenMeshCube(1.0f, 0, 1.0f));
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

BoundingBox ReBuildCollider(Model model, Vector3 position)
{
	BoundingBox local = GetMeshBoundingBox(model.meshes[0]);

	local.min = Vector3Add(local.min, position);
	local.max = Vector3Add(local.max, position);

	return local;
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

void RenderProps(const Props* obj) {
	for (size_t i = 0; i < obj->count; i++) {
		if ((obj->components[i] & PROP_LENS)) continue;
        if (!(obj->components[i] & PROP_VISIBILE)) continue;

        Vector3 scale = obj->size[i];
        if (scale.x == 0.0f && scale.y == 0.0f && scale.z == 0.0f) {
            scale = (Vector3){ 1.0f, 1.0f, 1.0f };
        }

        DrawModelEx(
            obj->model[i],
            obj->position[i],
            (Vector3){ 0.0f, 1.0f, 0.0f }, //rot axis
            0.0f, //rot angle
            scale,
            obj->color[i]
        );
	}
}

void RenderLensProps(const Props* obj) {
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
            (Vector3){ 0.0f, 1.0f, 0.0f }, //rot axis
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