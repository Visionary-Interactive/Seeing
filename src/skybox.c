#include "skybox.h"

Skybox* skybox;

void InitSkybox(const char *mapDir, bool hdr)
{
    skybox = (Skybox*)malloc(sizeof(Skybox));
    skybox->useHdr = hdr;

    char skyvsPath[512], skyfsPath[512], cubevsPath[512], cubefsPath[512], skyboxPath[512];
    snprintf(skyvsPath, sizeof(skyvsPath), "resources/shaders/skybox.vs", mapDir);
    snprintf(skyfsPath, sizeof(skyfsPath), "resources/shaders/skybox.fs", mapDir);
    snprintf(cubevsPath, sizeof(cubevsPath), "resources/shaders/cubemap.vs", mapDir);
    snprintf(cubefsPath, sizeof(cubefsPath), "resources/shaders/cubemap.fs", mapDir);

    Model model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

	model.materials[0].shader = LoadShader(skyvsPath, skyfsPath);

	SetShaderValue(model.materials[0].shader, GetShaderLocation(model.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
    SetShaderValue(model.materials[0].shader, GetShaderLocation(model.materials[0].shader, "doGamma"), (int[1]) { 0 }, SHADER_UNIFORM_INT);
    SetShaderValue(model.materials[0].shader, GetShaderLocation(model.materials[0].shader, "vflipped"), (int[1]){ 1 }, SHADER_UNIFORM_INT);
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture;

	Shader shaderCubemap = LoadShader(cubevsPath, cubefsPath);
	
	SetShaderValue(shaderCubemap, GetShaderLocation(shaderCubemap, "equirectangularMap"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

	char skyboxFileName[256] = {0};

    //this loads an equirectangular panorama
    if (!skybox->useHdr)
    {
        snprintf(skyboxPath, sizeof(skyboxPath), "%s/skybox2.png", mapDir);
        Image img = LoadImage(skyboxPath);
        model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
        UnloadImage(img);
    }
    //alternatively, if we want, we can load an hdr file
	else
    {
        snprintf(skyboxPath, sizeof(skyboxPath), "%s/skybox.hdr", mapDir);
        Texture2D panorama = LoadTexture(skyboxPath);
        model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = GenTextureCubemap(shaderCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        UnloadTexture(panorama);
    }

	M_ASSERT(model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture.id != 0, "Failed to generate cubemap texture\n"); 

    skybox->model = model;
}

Skybox* GetSkybox()
{
    return skybox;
}


static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format)
{
    TextureCubemap cubemap = {0};

    rlDisableBackfaceCulling();

    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(0, size, format, 1);

    unsigned int fbo = rlLoadFramebuffer();
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

    if (rlFramebufferComplete(fbo)) TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);

    rlEnableShader(shader.id);

    Matrix matFboProjection = MatrixPerspective(90.0*DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
    rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

    Matrix fboViews[6] = {
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  1.0f,  0.0f,  0.0f }, (Vector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ -1.0f,  0.0f,  0.0f }, (Vector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f,  1.0f,  0.0f }, (Vector3){ 0.0f,  0.0f,  1.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f, -1.0f,  0.0f }, (Vector3){ 0.0f,  0.0f, -1.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f,  0.0f,  1.0f }, (Vector3){ 0.0f, -1.0f,  0.0f }),
        MatrixLookAt((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){  0.0f,  0.0f, -1.0f }, (Vector3){ 0.0f, -1.0f,  0.0f })
    };

    rlViewport(0, 0, size, size);

    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; i++)
    {
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);
        
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        rlEnableFramebuffer(fbo);

        rlClearScreenBuffers();
        rlLoadDrawCube();
    }

    rlDisableShader();
    rlDisableTexture();
    rlDisableFramebuffer();
    rlUnloadFramebuffer(fbo);

    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = format;

    return cubemap;
}

void DestroySkybox()
{
    RL_FREE(skybox);
}