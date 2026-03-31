#pragma once
#ifndef IMPAIRMENT_H
#define IMPAIRMENT_H

#include "includes.h"
#include "player.h"

typedef enum {
    Tritanopia,
    Astigmatism,
    Convex,
    Glaucoma
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

    //glaucoma
    float locTunnelRadius;
    float locBlurStrength;
    float locContrastLoss;
    float locEdgeDarkening;

    //convex lens
    int locRefractiveIndex;
    int locLensStrength;

    //runtime parameters
    float intensity;
	float offsetIntensity; //used for temporary adjustments

    float angle;
    float radiusMajor;
    float radiusMinor;
    int samples;

    //these are for convex but we could probably consolidate these
    float refractiveIndex;
    float lensStrength;
    
    //glau
    float tunnelRadius;
    float contrastLoss;
    float blurStrength;
    float edgeDarkening;



} Impairment;

//screensize changes, so we need to pass this when we render the quad
Impairment *LoadImpairment(Impairments type, int screenW, int screenH);
void BeginImpairment(Impairment *vs);

void IncreaseImpairmentIntensity(Impairment* vs);
void EndImpairment(Impairment *vs);
void UpdateImpairment(Impairment *vs, float Intensity);
void DestroyImpairment(Impairment *vs);

#endif
