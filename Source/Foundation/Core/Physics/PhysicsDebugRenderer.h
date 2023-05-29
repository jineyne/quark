#pragma once

#include "CorePrerequisites.h"
#include "Renderer/RendererExtension.h"
#include "Material/Material.h"
#include "Material/GpuParamsSet.h"
#include "Collider.h"

class DLL_EXPORT PhysicsDebugRenderer : public RendererExtension {
private:
    TArray<Collider *> mRegisteredColliderList;

    Material *mMaterial = nullptr;
    GpuParamsSet *mParams = nullptr;

public:
    PhysicsDebugRenderer();

public:
    void initialize(const std::any &any) override;
    void render(CameraBase *camera) override;
    bool checkCamera(CameraBase *camera) override;
    void update(CameraBase *camera);

    void notifyColliderCreated(Collider *collider);
    void notifyColliderRemoved(Collider *collider);

private:
    friend class Physics;
};
