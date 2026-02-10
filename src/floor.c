#include "floor.h"

static Floor dFloorStorage;
static Floor* dFloor = &dFloorStorage;

static void ScaleTexcoords(Mesh* mesh, float tileRepeat)
{
    if (!mesh->texcoords) return;

    for (int i = 0; i < mesh->vertexCount * 2; i += 2)
    {
        mesh->texcoords[i]     *= tileRepeat;
        mesh->texcoords[i + 1] *= tileRepeat;
    }
}

bool InitFloor(const char* texturePath, float width, float length, float tileRepeat)
{
    Mesh plane = GenMeshPlane(width, length, 1, 1);
    ScaleTexcoords(&plane, tileRepeat);

    dFloor->model = LoadModelFromMesh(plane);
    dFloor->texture = LoadTexture(texturePath);
    SetTextureFilter(dFloor->texture, TEXTURE_FILTER_BILINEAR);

    dFloor->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = dFloor->texture;
    SetTextureWrap(dFloor->texture, TEXTURE_WRAP_REPEAT);
    return true;
}

Floor* GetFloor(void)
{
    return dFloor;
}

void DrawFloor(Vector3 position, Color tint)
{
    if (!dFloor->model.meshes) return;

    rlPushMatrix();
    rlTranslatef(position.x, position.y, position.z);

    DrawMesh(dFloor->model.meshes[0],
             dFloor->model.materials[0],
             MatrixIdentity());

    rlPopMatrix();
}

void UnloadFloor(void)
{
    if (!dFloor->model.meshes) return;
    UnloadModel(dFloor->model);
    UnloadTexture(dFloor->texture);
    *dFloor = (Floor){0};
}