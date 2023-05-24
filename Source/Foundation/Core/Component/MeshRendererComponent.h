#pragma once

#include "CorePrerequisites.h"
#include "Renderer/Renderable.h"
#include "Scene/Component.h"
#include "MeshRendererComponent.g.h"

QCLASS()
class DLL_EXPORT FMeshRendererComponent : public FComponent {
    GENERATED_BODY();

private:
    FRenderable *mInternal;

    FResourceHandle<FMesh> mMesh = nullptr;
    FMaterial *mMaterial = nullptr;

public:
    void onCreate() override;
    void onDestroy() override;

    void onStart() override;
    void onUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    void setMesh(const FResourceHandle<FMesh> &mesh);
    const FResourceHandle<FMesh> &getMesh() const;

    void setMaterial(FMaterial *material);
    FMaterial *getMaterial() const;
};