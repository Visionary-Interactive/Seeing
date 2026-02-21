#include "particleEmitter.h"

static ParticleEmitter emitterStorage;
static ParticleEmitter* emitter_t = &emitterStorage;

ParticleTemplate flameTemplate;

void InitFlameTemplate(void) {
    flameTemplate.position = (Vector3){ 10.0f, 3.0f, 10.0f };
    flameTemplate.velocity = (Vector3){ 0.0f, 5.0f, 0.0f };
    flameTemplate.size = (Vector3){ 5.0f, 5.0f, 1.0f };
    flameTemplate.lifetime = 1.5f;
}

ParticleEmitter* GetParticleEmitter(void) {
    return emitter_t;
}

ParticleTemplate* GetParticleTemplate(void) {
    return &flameTemplate;
}

void InitParticleEmitter(ParticleEmitter* emitter, ParticlePool* pool, float emissionRate) {
    if (!emitter) return;
    InitFlameTemplate();
    emitter->pool = pool;
    emitter->emissionRate = emissionRate;
    emitter->emissionAccumulator = 0.0f;
}

void UpdateParticleEmitter(ParticleEmitter* emitter, const ParticleTemplate* template, float dt) {
    if (!emitter || !emitter->pool) return;

    emitter->emissionAccumulator += emitter->emissionRate*dt;
    int emitCount = (int)emitter->emissionAccumulator;
    emitter->emissionAccumulator -= emitCount;

    for (int i = 0; i < emitCount; ++i) {
        SpawnParticle(emitter->pool, template->position, template->velocity, template->size,
            template->lifetime);
    }

    UpdateParticlePool(emitter->pool, dt);
}

void RenderParticlePool(const ParticlePool* pool, Camera3D camera, Color tint) {
    if (!pool || !pool->particles || pool->capacity == 0) return;
    if (pool->texture.id == 0) return;

    Rectangle source = { 0.0f, 0.0f, (float)pool->texture.width, (float)pool->texture.height };
    Vector3 up = { 0.0f, 1.0f, 0.0f };

    rlDrawRenderBatchActive(); //flush pending geometery
    rlDisableDepthMask(); //stop writing depth

    BeginBlendMode(BLEND_ALPHA);
    
    for (size_t i = 0; i < pool->capacity; ++i) {
        const Particle* particle = &pool->particles[i];
        if (!particle->active) continue;

        Vector2 quadSize = { particle->size.x, particle->size.y > 0 ? particle->size.y : particle->size.x };
        if (quadSize.x <= 0.0f || quadSize.y <= 0.0f) continue;

        Vector2 origin = { quadSize.x * 0.5f, quadSize.y * 0.5f };
        DrawBillboardPro(camera, pool->texture, source, particle->position, up, quadSize, origin, 0.0f, tint);
    }

    EndBlendMode();

    rlDrawRenderBatchActive();      // flush blended billboards
    rlEnableDepthMask();
}