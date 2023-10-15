#pragma once

#include "CorePrerequisites.h"
#include "Renderer/Renderable.h"
#include "Scene/Component.h"
#include "SpriteRendererComponent.g.h"


QCLASS()
class DLL_EXPORT SpriteRendererComponent : public Component {
    GENERATED_BODY()

private:
    static Material *SpriteMaterial;

    Renderable *mInternal;

    FResourceHandle<Mesh> mMesh = nullptr;
    Material *mMaterial = nullptr;

public:
    void onCreate() override;
    void onDestroy() override;

    void onStart() override;
    void onUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    void setMesh(const FResourceHandle<Mesh> &mesh);
    const FResourceHandle<Mesh> &getMesh() const;

    void setMaterial(Material *material);
    Material *getMaterial() const;

    void setTexture(const FResourceHandle<Texture> &texture);
};
