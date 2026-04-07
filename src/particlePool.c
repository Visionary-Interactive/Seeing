#include "particlePool.h"

ParticlePool* InitParticlePool(size_t capacity) {
    if (capacity == 0) return NULL;

    ParticlePool* pool = (ParticlePool*)malloc(sizeof(ParticlePool));
    M_ASSERT(pool, "Could not allocate space for particle pool. It's hosed.");

    //change this later
    pool->texture = LoadTexture("resources/global/tex/flame3.png");

    pool->particles = (Particle*)calloc(capacity, sizeof(Particle));
    pool->freeList = (int*)malloc(sizeof(int) * capacity);

    if (!pool->particles || !pool->freeList) {
        RL_FREE(pool->particles);
        RL_FREE(pool->freeList);
        UnloadTexture(pool->texture);
        RL_FREE(pool);
        return NULL;
    }

    pool->capacity = capacity;
    pool->freeCount = capacity;

    for (size_t i = 0; i < capacity; ++i) {
        pool->freeList[i] = (int)(capacity - 1 - i);
        ResetParticle(&pool->particles[i]);
    }

    return pool;
}

void DestroyParticlePool(ParticlePool* pool) {
    if (!pool) return;
    RL_FREE(pool->particles);
    RL_FREE(pool->freeList);
    UnloadTexture(pool->texture);
    RL_FREE(pool);
}

void ResetParticlePool(ParticlePool* pool)
{
    if (!pool) return;

    pool->freeCount = pool->capacity;

    for (size_t i = 0; i < pool->capacity; ++i)
    {
        // Reset particle data
        ResetParticle(&pool->particles[i]);

        // Rebuild free list (same pattern as init)
        pool->freeList[i] = (int)(pool->capacity - 1 - i);
    }
}


void ResetParticle(Particle* particle) {
    memset(particle, 0, sizeof(*particle));
    particle->active = false;
}

int SpawnParticle(ParticlePool* pool, Vector3 position, Color tint, Vector3 velocity, Vector3 size, float lifetime) {
    if (!pool || pool->freeCount == 0) return -1;

    int index = pool->freeList[--pool->freeCount];
    Particle* particle = &pool->particles[index];

    float spread = 0.5f; // tweak per emitter/template
    Vector3 randomOffset = {
        ((float)GetRandomValue(-1000, 1000) / 1000.0f) * spread,
        ((float)GetRandomValue(-1000, 1000) / 1000.0f) * spread,
        ((float)GetRandomValue(-1000, 1000) / 1000.0f) * spread
    };

    particle->position = Vector3Add(position, randomOffset);
    particle->velocity = velocity;
    particle->tint = tint;
    particle->lifetime = lifetime;
    particle->size = size;
    particle->age = 0.0;
    particle->active = true;

    return index;
}

void UpdateParticlePool(ParticlePool* pool, float dt) {
    TraceLog(LOG_DEBUG, "\nWe have space left");
    if (!pool || dt <= 0.0f) return;

    for (size_t i = 0; i < pool->capacity; ++i) {
        Particle* particle = &pool->particles[i];
        if (!particle->active) continue;

        particle->age += dt;
        if (particle->age >= particle->lifetime) {
            DestroyParticle(pool, (int)i);
            continue;
        }

        particle->position.x += particle->velocity.x * dt;
        particle->position.y += particle->velocity.y * dt;
        particle->position.z += particle->velocity.z * dt;

        float lifeRatio = 1.0f - (particle->age / particle->lifetime);
        //particle->a = fmaxf(0.0f, lifeRatio);
    }
}

void DestroyParticle(ParticlePool* pool, int handle) {
    if (!pool || handle < 0 || (size_t)handle >= pool->capacity) return;
    Particle* particle = &pool->particles[handle];
    if (!particle->active) return;

    particle->active = false;
    pool->freeList[pool->freeCount++] = handle;
}