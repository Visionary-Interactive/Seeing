#include "lens.h"

static LensShaderData gLens;

void InitLensShader(int screenW, int screenH, RenderTexture2D sceneColorRT) {
    gLens.shader = LoadShader("resources/shaders/convex.vs",
                              "resources/shaders/convex.fs");

    int locResolution = GetShaderLocation(gLens.shader, "resolution");
    float resolution[2] = { (float)screenW, (float)screenH };
    SetShaderValue(gLens.shader, locResolution, resolution, SHADER_UNIFORM_VEC2);

    gLens.locSceneTex = GetShaderLocation(gLens.shader, "sceneTex");
    gLens.locCameraPos = GetShaderLocation(gLens.shader, "cameraPosition");
    gLens.locRefractiveIndex = GetShaderLocation(gLens.shader, "refractiveIndex");
    gLens.locLensStrength = GetShaderLocation(gLens.shader, "lensStrength");
    gLens.locTintColor = GetShaderLocation(gLens.shader, "tintColor");
    gLens.locTintStrength = GetShaderLocation(gLens.shader, "tintStrength");
    gLens.locAlpha = GetShaderLocation(gLens.shader, "alpha");

    float refrIdx = 1.15f;
    float bend = 0.10f;
    float tintMix = 0.35f;
    float alpha = 0.6f;
    Vector3 tintRGB = { 0.85f, 0.95f, 1.0f };

    SetShaderValue(gLens.shader, gLens.locRefractiveIndex, &refrIdx, SHADER_UNIFORM_FLOAT);
    SetShaderValue(gLens.shader, gLens.locLensStrength, &bend, SHADER_UNIFORM_FLOAT);
    SetShaderValue(gLens.shader, gLens.locTintStrength, &tintMix, SHADER_UNIFORM_FLOAT);
    SetShaderValue(gLens.shader, gLens.locAlpha, &alpha, SHADER_UNIFORM_FLOAT);
    SetShaderValue(gLens.shader, gLens.locTintColor, &tintRGB, SHADER_UNIFORM_VEC3);

    SetShaderValueTexture(gLens.shader, gLens.locSceneTex, sceneColorRT.texture);
}

void SetPropShader(Props* obj, int id, Shader shader) {
    obj->shader[id] = shader;
    for (int m = 0; m < obj->model[id].materialCount; m++) {
        obj->model[id].materials[m].shader = shader;
    }
}

int CreateLensProp(Props* props, Vector3 position, Vector3 size) {
    int id = props->count++;
    id = CreatePropPrimitive(props, PRIMITIVE_MODEL_LENS, position, size, WHITE, PROP_VISIBILE | PROP_LENS);
    SetPropShader(props, id, gLens.shader);
    return id;
}

void UpdateLensShader(const Camera* cam, RenderTexture2D sceneColorRT) {
    SetShaderValueTexture(gLens.shader, gLens.locSceneTex, sceneColorRT.texture);
    Vector3 camPos = cam->position;
    SetShaderValue(gLens.shader, gLens.locCameraPos, &camPos, SHADER_UNIFORM_VEC3);
}