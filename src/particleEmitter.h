#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "particlePool.h"

typedef struct {
    ParticlePool* pool;
    void* userData;
    float emissionRate;
    float emissionAccumulator;
} ParticleEmitter;

typedef struct {
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    float lifetime;
} ParticleTemplate;

void InitParticleEmitter(ParticleEmitter* system, ParticlePool* pool, float emissionRate);

void InitFlameTemplate(void);
ParticleEmitter* GetParticleEmitter(void);
ParticleTemplate* GetParticleTemplate(void);
void UpdateParticleEmitter(ParticleEmitter* system, const ParticleTemplate* template, float dt);
void RenderParticlePool(const ParticlePool* pool, Camera3D camera, Color tint);

#endif