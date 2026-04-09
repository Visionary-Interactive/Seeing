#ifndef PARTICLE_POOL_H
#define PARTICLE_POOL_H

#include "particle.h"

typedef struct {
    Particle* particles;
    Texture2D texture;
    int* freeList;
    size_t capacity;
    size_t freeCount;
} ParticlePool;

#ifndef PARTICLE_DEBUG
#define PARTICLE_DEBUG 1
#endif

#if PARTICLE_DEBUG
    #define PARTICLE_LOG(...) printf(__VA_ARGS__), printf("\n")
#else
    #define PARTICLE_LOG(...) ((void)0)
#endif

typedef void (*ParticleVisitor)(const Particle* particle, void* userData);

ParticlePool* InitParticlePool(size_t capacity);
void DestroyParticlePool(ParticlePool* pool);
void ResetParticlePool(ParticlePool* pool);
void ResetParticle(Particle* particle);
int SpawnParticle(ParticlePool* pool, Vector3 position, Color tint, Vector3 velocity, Vector3 size, float lifetime);
void UpdateParticlePool(ParticlePool* pool, float dt);
void DestroyParticle(ParticlePool* pool, int handle);
void ResetParticlePool(ParticlePool* pool);

#endif