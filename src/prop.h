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

typedef struct Props {
	size_t count;

	//core features
	Vector3 position[MAX_PROPS];
	Vector3 size[MAX_PROPS];
	Model model[MAX_PROPS];
	Color color[MAX_PROPS];
	Vector3 InteractRange[MAX_PROPS];

	//light data
	Color lightColor[MAX_PROPS];
	float lightIntensity[MAX_PROPS];

	//using bitmasks
	uint32_t components[MAX_PROPS];
	BoundingBox collider[MAX_PROPS];
	int interactType[MAX_PROPS];
	int scriptID[MAX_PROPS];

} Props;

typedef enum InteractableType {
	INTERACTABLE_NONE,
	INTERACTABLE_DOOR,
	INTERACTABLE_PICKUP,
} InteractableType;

void CreatePropStructure();
Props* GetPropStructure();
int CreateProp(Props* obj, Vector3 position, Vector3 size, Model model, Color color,uint32_t components);
void CreateLight(Props* obj, int id, Color color, float intensity);
void ColliderSetup(Props* obj, int id);
void RenderProps(const Props* obj);

#endif
