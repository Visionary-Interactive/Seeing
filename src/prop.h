#pragma once
#ifndef PROP_H
#define PROP_H
#include "includes.h"

#define MAX_PROPS 128
#define PROP_VISIBILE (1 << 0)
#define PROP_LIGHT (1 << 1)
#define PROP_COLLIDER (1 << 2)
#define PROP_SCRIPTED (1 << 3)
#define PROP_INTERACTABLE (1 << 4)
#define PROP_PUSHABLE (1 << 5)
#define PROP_DOOR (1 << 6)
#define PROP_PICKUP (1 << 7)
#define PROP_LENS (1 << 8)

#define PROP_MODEL_PATH_MAX 128

typedef enum {
	NO_PRIM = 0,
    PRIMITIVE_MODEL_CUBE,
    PRIMITIVE_MODEL_DOOR,
	PRIMITIVE_MODEL_PLATFORM,
	PRIMITIVE_MODEL_WALL,
	PRIMITIVE_MODEL_BUTTON,
    PRIMITIVE_MODEL_LENS
} PrimitiveModelId;

typedef enum InteractableType {
	INTERACTABLE_NONE = 0,
	INTERACTABLE_DOOR,
	INTERACTABLE_PICKUP,
} InteractableType;

typedef struct Props {
	size_t count;

	//core features
	Vector3 position[MAX_PROPS];
	Vector3 size[MAX_PROPS];
	Model model[MAX_PROPS];
	Color color[MAX_PROPS];
	Vector3 interactRange[MAX_PROPS];

	//light data
	Color lightColor[MAX_PROPS];
	float lightIntensity[MAX_PROPS];

	//using bitmasks
	uint32_t components[MAX_PROPS];
	BoundingBox collider[MAX_PROPS];
	int interactType[MAX_PROPS];
	int scriptID[MAX_PROPS];

	Shader shader[MAX_PROPS];
	PrimitiveModelId prim[MAX_PROPS];
	char modelPath[MAX_PROPS][PROP_MODEL_PATH_MAX];
	char texPath[MAX_PROPS][PROP_MODEL_PATH_MAX];

} Props;

void CreatePropStructure(void);
Props* GetPropStructure(void);
int CreateProp(Props* obj, Model model, Vector3 position, Vector3 size, Color color, uint32_t components);
int CreatePropPrimitive(Props* obj, PrimitiveModelId prim, Vector3 position, Vector3 size, Color color, uint32_t components);
void CreateLight(Props* obj, int id, Color color, float intensity);
void ColliderSetup(Props* obj, int id);
//rebuilds the collider for a prop based on its model and position
BoundingBox ReBuildCollider(Model model, Vector3 position);
void AddPropComponent(Props* obj, int id, uint32_t componentMask);
void RemovePropComponent(Props* obj, int id, uint32_t componentMask);
void RenderProps(const Props* obj);
void RenderLensProps(const Props* obj);
void ResetProps();
void DestroyProps(Props* obj);

#endif
