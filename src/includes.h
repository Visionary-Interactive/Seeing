#pragma once
#ifndef STANDARDINCLUDES_H
#define STANDARDINCLUDES_H

#if defined(_WIN32) || defined(_WIN64)
#else
#include <unistd.h> 
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include "rlgl.h"

#define MAJOR_VERSION 1
#define MINOR_VERSION 0

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else   //android or web...
    #define GLSL_VERSION 100
#endif

#define M_ASSERT(_cond, _msg) \
    if (!(_cond)) { printf(_msg); exit(1); }

#endif