#include "prop.h"

static Props* props;

void CreatePropStructure()
{
	props = (Props*)malloc(sizeof(Props));
	memset(props, 0, sizeof(Props));
}

Props* GetPropStructure()
{
	return props;
}

int CreateProp(Props* obj, Vector3 position, Vector3 size, Model model, Color color ,uint32_t components) {
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
	return id;
}

void CreateLight(Props* obj, int id, Color color, float intensity) {
	if (id < 0 || id >= obj->count) {
		return; // Invalid ID
	}
	obj->lightColor[id] = color;
	obj->lightIntensity[id] = intensity;
	obj->components[id] |= PROP_LIGHT; // Add light component
}

void ColliderSetup(Props* obj, int id) {
	if (id < 0 || id >= obj->count) {
		return; // Invalid ID
	}
	//get the boundingbox from the model, adds scale to it 
	BoundingBox bb = GetModelBoundingBox(obj->model[id]);
	bb.min.x *= obj->size[id].x;  bb.min.y *= obj->size[id].y;  bb.min.z *= obj->size[id].z;
	bb.max.x *= obj->size[id].x;  bb.max.y *= obj->size[id].y;  bb.max.z *= obj->size[id].z;

	//offsets the box based on the position
	bb.min = Vector3Add(bb.min, obj->position[id]);
	bb.max = Vector3Add(bb.max, obj->position[id]);

	obj->collider[id] = bb;

}

void RenderProps(const Props* obj) {
	for (size_t i = 0; i < obj->count; i++) {
		if (obj->components[i] & PROP_LENS) continue;
		if (obj->components[i] & PROP_VISIBILE ) {
			DrawModel(obj->model[i], obj->position[i], 1.0f, obj->color[i]);
		}
	}
}

void RenderLensProps(const Props* obj) {
	for (size_t i = 0; i < obj->count; i++) {
		if (!(obj->components[i] & PROP_LENS)) continue;
		if (obj->components[i] & PROP_VISIBILE ) {
			DrawModel(obj->model[i], obj->position[i], 1.0f, obj->color[i]);
		}
	}
}