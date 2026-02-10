#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "includes.h"

typedef struct Skybox {
    Model model;
    bool useHdr;
} Skybox;

void InitSkybox(const char *mapDir, bool hdr);
Skybox* GetSkybox();
static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format);
void DestroySkybox();

#endif