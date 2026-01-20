#pragma once
#ifndef LENS_H
#define LENS_H

#include "includes.h"
#include "prop.h"

typedef struct LensShaderData {
    Shader shader;
    int locSceneTex;
    int locCameraPos;
    int locRefractiveIndex;
    int locLensStrength;
    int locTintColor;
    int locTintStrength;
    int locAlpha;
} LensShaderData;

void InitLensShader(int screenW, int screenH, RenderTexture2D sceneColorRT);
void SetPropShader(Props* obj, int id, Shader shader);
int CreateLensProp(Props* props, Vector3 position, Vector3 size);
void UpdateLensShaderPerFrame(const Camera* cam, RenderTexture2D sceneColorRT);

#endif