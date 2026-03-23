#include "raylib.h"
#include "impairment.h"
#include <string.h>

static Shader LoadImpairmentFile(Impairments type)
{
    switch (type)
    {
        case Astigmatism:
            return LoadShader("resources/shaders/impairment.vs", 
                              "resources/shaders/astigmatism.fs");
        case Tritanopia:
            return LoadShader("resources/shaders/impairment.vs", 
                              "resources/shaders/tritanopia.fs");
        case Convex:
            return LoadShader("resources/shaders/impairment.vs", 
                              "resources/shaders/convex.fs");
        case Glaucoma:
            return LoadShader("resources/shaders/impairment.vs", 
                              "resources/shaders/glaucoma.fs");
        default:
            TraceLog(LOG_WARNING, "Unknown impairment type...");
            return LoadShader(0, 0);
    }
}

Impairment *LoadImpairment(Impairments type, int screenW, int screenH)
{
    Impairment *im = malloc(sizeof(Impairment));
    memset(im, 0, sizeof(Impairment));

    im->type = type;
    im->screenW = screenW;
    im->screenH = screenH;
    im->shader = LoadImpairmentFile(type);
    im->target = LoadRenderTexture(screenW, screenH);

    //astig
    im->intensity = 0.4f;
    im->angle = 0.0f;
    im->radiusMajor = 6.0f;
    im->radiusMinor = 1.5f;
    im->samples = 15;

    //glaucoma
    im->tunnelRadius = 0.35f;
    im->blurStrength = 2.0f;
    im->contrastLoss = 0.4f;
    im->edgeDarkening = 0.8f;

    im->refractiveIndex = 1.15f;
    im->lensStrength = 0.35f;

    im->locIntensity = GetShaderLocation(im->shader, "intensity");
    im->locResolution = GetShaderLocation(im->shader, "resolution");
    float res[2] = { (float)screenW, (float)screenH };
    SetShaderValue(im->shader, im->locResolution, res, SHADER_UNIFORM_VEC2);

    if (type == Astigmatism)
    {
        im->locAngle = GetShaderLocation(im->shader, "angle");
        im->locRMajor = GetShaderLocation(im->shader, "radiusMajor");
        im->locRMinor = GetShaderLocation(im->shader, "radiusMinor");
        im->locSamples = GetShaderLocation(im->shader, "samples");
    }

    if (type == Convex)
    {
        im->locRefractiveIndex = GetShaderLocation(im->shader, "refractiveIndex");
        im->locLensStrength = GetShaderLocation(im->shader, "lensStrength");
    }

    if (type == Glaucoma)
    {
        im->locTunnelRadius = GetShaderLocation(im->shader, "tunnelRadius");
        im->locBlurStrength = GetShaderLocation(im->shader, "blurStrength");
        im->locEdgeDarkening = GetShaderLocation(im->shader, "edgeDarkening");
        im->locContrastLoss = GetShaderLocation(im->shader, "contrastLoss");
    }

    return im;
}

// This dual function solution is sort of clumsy for now and I would rather this not be exposed to main.c but we'll likely end up
// consolidating the drawing process to just use one function in map.c in the object of avoiding main() do draw calls directly.
void BeginImpairment(Impairment *im)
{
    BeginTextureMode(im->target);
    ClearBackground(RAYWHITE);
}

void SetImpairment(Impairment* vs, float value)
{
    if (!vs) return;

    if (value < 0.0f) value = 0.0f;
    if (value > 2.0f) value = 2.0f;

    vs->intensity = value;

}

void EndImpairment(Impairment *im)
{
    if (!im) return;

    SetShaderValue(im->shader, im->locIntensity, &im->intensity, SHADER_UNIFORM_FLOAT);

    if (im->type == Astigmatism)
    {
        SetShaderValue(im->shader, im->locAngle,   &im->angle, SHADER_UNIFORM_FLOAT);
        SetShaderValue(im->shader, im->locRMajor,  &im->radiusMajor, SHADER_UNIFORM_FLOAT);
        SetShaderValue(im->shader, im->locRMinor,  &im->radiusMinor, SHADER_UNIFORM_FLOAT);
        SetShaderValue(im->shader, im->locSamples, &im->samples, SHADER_UNIFORM_INT);
    }

    if (im->type == Convex)
    {
        SetShaderValue(im->shader, im->locRefractiveIndex, &im->refractiveIndex, SHADER_UNIFORM_FLOAT);
        SetShaderValue(im->shader, im->locLensStrength, &im->lensStrength, SHADER_UNIFORM_FLOAT);
    }

    if (im->type == Glaucoma)
    {
        SetShaderValue(im->shader, im->locTunnelRadius, &im->tunnelRadius, SHADER_UNIFORM_FLOAT);
        SetShaderValue(im->shader, im->locBlurStrength, &im->blurStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(im->shader, im->locEdgeDarkening, &im->edgeDarkening, SHADER_UNIFORM_FLOAT);
        SetShaderValue(im->shader, im->locContrastLoss, &im->contrastLoss, SHADER_UNIFORM_FLOAT);
    }

    EndTextureMode();

    BeginShaderMode(im->shader);
    DrawTextureRec(im->target.texture,
        (Rectangle){ 0, 0, (float)im->screenW, -(float)im->screenH },
        (Vector2){ 0, 0 }, WHITE);
    EndShaderMode();
}

void IncreaseImpairmentIntensity(Impairment *im)
{
    if (!im) return;
    im->intensity += 0.5f;
}

void UpdateImpairment(Impairment *im, float intensity)
{
    if (!im) return;

    //universal controls
    if (IsKeyDown(KEY_UP))    im->intensity += 0.01f;
    if (IsKeyDown(KEY_DOWN))  im->intensity -= 0.01f;

	im->intensity += intensity;

    if (im->type == Astigmatism)
    {
        if (IsKeyDown(KEY_RIGHT)) im->angle += 0.02f;
        if (IsKeyDown(KEY_LEFT))  im->angle -= 0.02f;

        if (IsKeyPressed(KEY_O)) {
            im->intensity = 0.4f;
            im->angle = 0.0f;
            im->radiusMajor = 4.0f;
            im->radiusMinor = 1.2f;
            im->samples = 11;
        }
        if (IsKeyPressed(KEY_P)) {
            im->intensity = 0.2f;
            im->angle = 0.0f;
            im->radiusMajor = 10.0f;
            im->radiusMinor = 2.0f;
            im->samples = 20;
        }
    }

    if (im->type == Convex)
    {
        if (IsKeyDown(KEY_RIGHT)) im->lensStrength += 0.01f;
        if (IsKeyDown(KEY_LEFT))  im->lensStrength -= 0.01f;

        if (IsKeyDown(KEY_O)) im->refractiveIndex += 0.01f;
        if (IsKeyDown(KEY_P)) im->refractiveIndex -= 0.01f;
    }
}

void DestroyImpairment(Impairment *im)
{
    M_ASSERT(im, "Tried to free non-existent impairment memory...")
    UnloadRenderTexture(im->target);
    UnloadShader(im->shader);
    RL_FREE(im);
}

