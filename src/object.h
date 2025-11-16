#pragma once
#ifndef OBJECT_H
#define OBJECT_H
#include "includes.h"

#define MAX_PROPS 1024
#define OBJECT_VISIBILE (1 << 0)
#define OBJECT_LIGHT (1 << 1)
#define OBJECT_COLLIDER (1 << 2)
#define OBJECT_SCRIPTED (1 << 3)
#define OBJECT_INTERACTABLE (1 << 4)
#define OBJECT_PUSHABLE (1 << 5)
#define OBJECT_DOOR (1 << 6)
#define OBJECT_PICKUP (1 << 7)

typedef struct GameObject {
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

} GameObject;

typedef enum InteractableType {
	INTERACTABLE_NONE,
	INTERACTABLE_DOOR,
	INTERACTABLE_PICKUP,
} InteractableType;


int CreateObject(GameObject* obj, Vector3 position, Vector3 size, Model model, Color color,uint32_t components);

void CreateLight(GameObject* obj, int id, Color color, float intensity);

void ColliderSetup(GameObject* obj, int id);

void RenderProps(const GameObject* obj);
#endif // !OBJECT_H
