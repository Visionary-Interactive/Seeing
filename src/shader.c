#include "raylib.h"
#include "shader.h"

static Shader shader;
static RenderTexture2D target;
static int screenW, screenH;

static int locResolution, locIntensity, locAngle, locRMajor, locRMinor, locSamples, locChroma;

static float intensity = 0.4f;
static float angle = 0.0f;
static float rMajor = 6.0f;
static float rMinor = 1.5f;
static int samples = 15;
static float chroma = 0.05f;

void InitVisionShader(int screenWidth, int screenHeight)
{
    screenW = screenWidth;
    screenH = screenHeight;

    target = LoadRenderTexture(screenWidth, screenHeight);
    shader = LoadShader("resources/shaders/impairment.vs", "resources/shaders/astigmatism.fs");

    locResolution = GetShaderLocation(shader, "resolution");
    float resolution[2] = { (float)screenWidth, (float)screenHeight };
    SetShaderValue(shader, locResolution, resolution, SHADER_UNIFORM_VEC2);

    locIntensity = GetShaderLocation(shader, "intensity");
    locAngle = GetShaderLocation(shader, "angle");
    locRMajor = GetShaderLocation(shader, "radiusMajor");
    locRMinor = GetShaderLocation(shader, "radiusMinor");
    locSamples = GetShaderLocation(shader, "samples");
    locChroma = GetShaderLocation(shader, "chromaDispersion");
}

void DestroyVisionShader(void)
{
    UnloadRenderTexture(target);
    UnloadShader(shader);
}

void BeginVisionRender(void)
{
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
}

void EndVisionRender(void)
{
    //update uniforms before applying...
    SetShaderValue(shader, locIntensity, &intensity, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, locAngle, &angle, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, locRMajor, &rMajor, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, locRMinor, &rMinor, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, locSamples, &samples, SHADER_UNIFORM_INT);
    SetShaderValue(shader, locChroma, &chroma, SHADER_UNIFORM_FLOAT);

    EndTextureMode();

    BeginShaderMode(shader);
    DrawTextureRec(target.texture,
        (Rectangle){ 0, 0, (float)screenW, -(float)screenH },
        (Vector2){ 0, 0 }, WHITE);
    EndShaderMode();
}

void UpdateVisionShader(float deltaTime)
{
    if (IsKeyDown(KEY_RIGHT)) angle += 0.02f;
        if (IsKeyDown(KEY_LEFT)) angle -= 0.02f;
        if (IsKeyDown(KEY_UP)) intensity += 0.01f;
        if (IsKeyDown(KEY_DOWN)) intensity -= 0.01f;
        if (IsKeyDown(KEY_O))
        {
            intensity = 0.4f;
            angle = 0.0f;
            rMajor = 4.0f;
            rMinor = 1.2f;
            samples = 11;
            chroma = 0.05f;
        }
        if (IsKeyDown(KEY_P))
        {
            intensity = 0.2f;
            angle = 0.0f;
            rMajor = 10.0f;
            rMinor = 2.0f;
            samples = 20;
            chroma = 0.05f;
        }
}
