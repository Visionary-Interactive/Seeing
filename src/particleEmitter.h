#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "particlePool.h"

#define MAX_EMITTERS 16

typedef struct {
    Vector3 velocity;
    Vector3 size;
    float lifetime;
} ParticleTemplate;

typedef struct {
    ParticlePool* pool;
    void* userData;
    float emissionRate;
    float emissionAccumulator;
    Vector3 position;
    ParticleTemplate* prefab;
    Color tint;
} ParticleEmitter;

ParticleEmitter* InitParticleEmitter(ParticlePool* pool, float emissionRate, 
    Vector3 position, ParticleTemplate* prefab, Color tint);
void InitFlameTemplate(void);
ParticleEmitter* GetParticleEmitter(void);
ParticleTemplate* GetParticleTemplate(void);
void UpdateParticleEmitter(float dt);
void RenderParticlePool(const ParticlePool* pool, Camera3D camera, Color tint);

#endif