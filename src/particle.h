#ifndef PARTICLE_H
#define PARTICLE_H

#include "includes.h"

typedef struct {
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    float lifetime;
    float age;
    bool active;
    Color tint;
} Particle;

#endif