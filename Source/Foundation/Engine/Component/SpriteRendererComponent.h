#pragma once

#include "Prerequisites.h"
#include "Renderer/Renderable.h"
#include "Image/Sprite.h"
#include "Scene/Component.h"
#include "SpriteRendererComponent.g.h"

QCLASS()
class DLL_EXPORT SpriteRendererComponent : public Component {
    GENERATED_BODY()

private:
    static Shader *SpriteShader;

    Renderable *mInternal;

    Sprite *mSprite;

    VertexDataDesc *mVertexDataDesc;
    MeshData *mMeshData = nullptr;
    Mesh *mMesh = nullptr;
    Material *mMaterial = nullptr;

    bool bIsDirty = false;

public:
    void onCreate() override;
    void onDestroy() override;

    void onStart() override;
    void onUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    Sprite *getSprite() const;
    void setSprite(Sprite *sprite);

    void setMaterial(Material *material);
    Material *getMaterial() const;
};
