#pragma once
#ifndef RENDER_H
#define RENDER_H

#include "includes.h"
#include "player.h"
#include "prop.h"
#include "camera.h"
#include "impairment.h"
#include "map.h"
#include "menu.h"
#include "sessionManager.h"
#include "sessionStateController.h"
#include "lens.h"
#include "render.h"
#include "floor.h"
#include "particleEmitter.h"
#include "particlePool.h"

static void UpdateSceneImpairments(float intensity);
void DestroySceneImpairments(void);

void InitSceneImpairments(int screenWidth, int screenHeight);
void DestroySceneImpairments(void);

void RenderSceneToTexture(MenuScreen currentScreen, RenderTexture2D sceneColorRT, Camera* camera, Props* props, ParticlePool* pool);
void RenderFinalFrame(MenuScreen currentScreen, RenderTexture2D sceneColorRT, Camera* camera, Props* props, int swap, float intensity,int screenWidth, int screenHeight);

#endif