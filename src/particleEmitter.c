#include "particleEmitter.h"

static ParticleEmitter gEmitters[MAX_EMITTERS] = { 0 };
static size_t gEmitterCount = 0;
ParticleTemplate flameTemplate;

void InitFlameTemplate(void) {
    flameTemplate.velocity = (Vector3){ 0.0f, 5.0f, 0.0f };
    flameTemplate.size = (Vector3){ 2.0f, 2.0f, 1.0f };
    flameTemplate.lifetime = 1.5f;
}

ParticleEmitter* GetParticleEmitter(void) {
    return (gEmitterCount > 0) ? &gEmitters[0] : NULL;
}

ParticleTemplate* GetParticleTemplate(void) {
    return &flameTemplate;
}

ParticleEmitter* InitParticleEmitter(ParticlePool* pool, float emissionRate, 
    Vector3 position, ParticleTemplate* prefab, Color tint) {
    if (!pool || !prefab) return NULL;
    if (gEmitterCount >= MAX_EMITTERS) return NULL;

    ParticleEmitter* emitter = &gEmitters[gEmitterCount++];
    emitter->pool = pool;
    emitter->emissionRate = emissionRate;
    emitter->emissionAccumulator = 0.0f;
    emitter->position = position;
    emitter->prefab = prefab;
    emitter->tint = tint;
    return emitter;
}

void SetEmitterPosition(ParticleEmitter* emitter, Vector3 position) {
    if (!emitter) return;
    emitter->position = position;
}

void UpdateParticleEmitter(float dt) {
    for (size_t i = 0; i < gEmitterCount; ++i) {
        ParticleEmitter* emitter = &gEmitters[i];
        if (!emitter->pool || !emitter->prefab) continue;

        ParticleTemplate* template = emitter->prefab;

        emitter->emissionAccumulator += emitter->emissionRate * dt;
        int emitCount = (int)emitter->emissionAccumulator;
        emitter->emissionAccumulator -= emitCount;

        for (int j = 0; j < emitCount; ++j) {
            SpawnParticle(emitter->pool,
                          emitter->position,
                          emitter->tint,
                          template->velocity,
                          template->size,
                          template->lifetime);
        }

        //UpdateParticlePool(emitter->pool, dt);
    }
}

void RenderParticlePool(const ParticlePool* pool, Camera3D camera, Color tint) {
    if (!pool || !pool->particles || pool->capacity == 0) return;
    if (pool->texture.id == 0) return;

    Rectangle source = { 0.0f, 0.0f, (float)pool->texture.width, (float)pool->texture.height };
    Vector3 up = { 0.0f, 1.0f, 0.0f };

    //rlDrawRenderBatchActive();
    rlDisableDepthMask();
    rlSetBlendFactors(RL_SRC_ALPHA, RL_ONE, RL_FUNC_ADD);
    rlSetBlendMode(BLEND_CUSTOM);
    
    for (size_t i = 0; i < pool->capacity; ++i) {
        const Particle* particle = &pool->particles[i];
        if (!particle->active) continue;

        Vector2 quadSize = { particle->size.x, particle->size.y > 0 ? particle->size.y : particle->size.x };
        if (quadSize.x <= 0.0f || quadSize.y <= 0.0f) continue;

        Vector2 origin = { quadSize.x * 0.5f, quadSize.y * 0.5f };
        DrawBillboardPro(camera, pool->texture, source, particle->position, up, quadSize, origin, 0.0f, particle->tint);
    }

    rlSetBlendMode(BLEND_ALPHA);
    //EndBlendMode();

    rlDrawRenderBatchActive();
    rlEnableDepthMask();
}

void ClearAllEmitters(void) {
    gEmitterCount = 0;
    
    for (size_t i = 0; i < MAX_EMITTERS; ++i) {
        gEmitters[i] = (ParticleEmitter){ 0 };
    }
}