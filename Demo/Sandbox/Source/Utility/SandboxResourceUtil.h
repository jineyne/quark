#pragma once

#include "Sandbox.h"
#include "FileSystem/Path.h"
#include "Material/Shader.h"
#include "Material/Material.h"
#include "Mesh/Mesh.h"

struct MaterialParam {
    Color globalAmbient = Color::Black;
    Color ambientColor = Color::Black;
    Color emissiveColor = Color::Black;
    Color diffuseColor = Color::Black;
    Color specularColor = Color::Black;

    Color reflectance = Color::Black;

    float opacity = 1.f;
    float specularPower = 0.5f;
    float indexOfRefraction = 2;
    BOOL hasAmbientTexture = FALSE;

    BOOL hasEmissiveTexture = FALSE;
    BOOL hasDiffuseTexture = FALSE;
    BOOL hasSpecularTexture = FALSE;
    BOOL hasSpecularPowerTexture = FALSE;

    BOOL hasNormalTexture = FALSE;
    BOOL hasBumpTexture = FALSE;
    BOOL hasOpacityTexture = FALSE;
    float bumpIntensity = 1;

    float specularScale = 2;
    float alphaThreshold = 3;
    float padding[2];
};

extern MaterialParam gMaterialParam;

class SandboxResourceUtil {
private:
    static Shader *ForwardRendering;

    static Material *RedShipMaterial;
    static Material *BlueShipMaterial;
    static Material *GreenShipMaterial;

    static Material *BulletMaterial;

    static FResourceHandle<Mesh> SparrowMesh;

public:
    static void Initialize();
    static void Finalization();

    static String Read(Path path);
    static Shader *LoadShader(Path path);

    static Material *GetRedShipMaterial() { return RedShipMaterial; }
    static Material *GetBlueShipMaterial() { return BlueShipMaterial; }
    static Material *GetGreenShipMaterial() { return GreenShipMaterial; }
    static Material *GetBulletMaterial() { return BulletMaterial; }

    static FResourceHandle<Mesh> GetSparrowMesh() { return SparrowMesh; }
};