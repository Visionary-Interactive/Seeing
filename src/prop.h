#pragma once
#ifndef PROP_H
#define PROP_H
#include "includes.h"
#include "TextboxType.h"
#include "particleEmitter.h"
#include "modelPool.h"
#include "texturePool.h"

#define MAX_PROPS 512
#define MAX_TEXT_LENGTH 512
#define PROP_VISIBILE (1 << 0)
#define PROP_LIGHT (1 << 1)
#define PROP_COLLIDER (1 << 2)
#define PROP_SCRIPTED (1 << 3)
#define PROP_INTERACTABLE (1 << 4)
#define PROP_PUSHABLE (1 << 5)
#define PROP_DOOR (1 << 6)
#define PROP_PICKUP (1 << 7)
#define PROP_LENS (1 << 8)
#define PROP_BUTTON (1 << 9)
#define PROP_DEADZONE (1 << 10)
#define PROP_CHECKPOINT (1 << 11)
#define PROP_VENTLID (1 << 12)
#define PROP_VENT (1 << 13)
#define PROP_TRIGGERZONE (1 << 14)

#define PROP_MODEL_PATH_MAX 512

typedef enum PrimitiveModelId {
	NO_PRIM = 0,
    PRIMITIVE_MODEL_CUBE,
    PRIMITIVE_MODEL_DOOR,
	PRIMITIVE_MODEL_PLATFORM,
	PRIMITIVE_MODEL_WALL,
	PRIMITIVE_MODEL_WALL2,
	PRIMITIVE_MODEL_BUTTON,
    PRIMITIVE_MODEL_LENS,
	PRIMITIVE_MODEL_WARP
} PrimitiveModelId;

typedef enum InteractableType {
	INTERACTABLE_NONE = 0,
	INTERACTABLE_DOOR,
	INTERACTABLE_PICKUP,
	INTERACTABLE_TEXT,
	INTERACTABLE_PUSH,
	INTERACTABLE_PUZZLE_ROTATATION_BLOCK,
	INTERACTABLE_VENTLID,
	INTERACTABLE_DISAPPEARING_WALL,
	INTERACTABLE_DISAPPEARING_WALL2
} InteractableType;

typedef enum TriggerType {
	TRIGGER_NONE = 0,
	TRIGGER_DEADZONE,
	TRIGGER_CHECKPOINT,
	TRIGGER_WARP,
	TRIGGER_TEXT,
	TRIGGER_IMPAIRMENT
} TriggerType;


typedef struct Props {
	size_t count;

	//core features
	Vector3 position[MAX_PROPS];
	Vector3 size[MAX_PROPS];
	Vector3 rotation[MAX_PROPS]; // RADIAN
	Model* model[MAX_PROPS];
	Color color[MAX_PROPS];
	Vector3 interactRange[MAX_PROPS];
	char* text[MAX_PROPS];
	TextboxType textType[MAX_PROPS];

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

	//trigger type for interactions (warp, buttons, trigger event)
	TriggerType triggerType[MAX_PROPS];
	bool Triggered[MAX_PROPS];

	int ImpairmentType[MAX_PROPS];

	Vector3 warpTarget[MAX_PROPS];

} Props;

extern int rotatingBlockIDs[6];

void CreatePropStructure(void);
Props* GetPropStructure(void);
ParticlePool* GetParticlePool(void);
int CreateProp(Props* obj, Model* model, Vector3 position, Vector3 size, Color color, uint32_t components);
int CreatePropPrimitive(Props* obj, PrimitiveModelId prim, Vector3 position, Vector3 size, Color color, uint32_t components);
int CreatePropFromPath(Props* obj, const char* modelPath, const char* texPath, Vector3 position, Vector3 size, Color color, uint32_t components);
void CreateLight(Props* obj, int id, Color color, float intensity);
void ColliderSetup(Props* obj, int id);
BoundingBox ReBuildCollider(Props* obj, int id, Vector3 position);

int AddZone(Props* obj, Vector3 position, Vector3 size, TriggerType type);
int AddKillFlame(Vector3 position, Vector3 size, bool deadly);
bool CheckCollisionWithProp(const Props* obj, int id, BoundingBox other);
void AddPropComponent(Props* obj, int id, uint32_t componentMask);
void RemovePropComponent(Props* obj, int id, uint32_t componentMask);
void RenderProps(Props* obj);

void RenderLensProps(const Props* obj);
void ResetProps();
void DestroyProps(Props* obj);

#endif
