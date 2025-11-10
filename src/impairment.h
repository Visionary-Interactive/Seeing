#pragma once
#ifndef IMPAIRMENT_H
#define IMPAIRMENT_H

#include "includes.h"

typedef enum {
    Tritanopia,
    Astigmatism
} Impairments;

typedef struct {
    Impairments type;
    Shader shader;
    RenderTexture2D target;
    int screenW, screenH;

    //common uniforms
    int locIntensity;
    int locResolution;

    //astigmatism
    int locAngle;
    int locRMajor;
    int locRMinor;
    int locSamples;

    //parameters
    float intensity;
    float angle;
    float radiusMajor;
    float radiusMinor;
    int samples;

} Impairment;

//screensize changes, so we need to pass this when we render the quad
Impairment *LoadImpairment(Impairments type, int screenW, int screenH);
void BeginImpairment(Impairment *vs);
void EndImpairment(Impairment *vs);
void UpdateImpairment(Impairment *vs);
void DestroyImpairment(Impairment *vs);

#endif
