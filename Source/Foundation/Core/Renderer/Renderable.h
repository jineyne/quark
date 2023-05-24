#pragma once

#include "CorePrerequisites.h"
#include "Mesh/Mesh.h"
#include "Material/Material.h"
#include "Scene/Transform.h"

class DLL_EXPORT FRenderable {
private:
    static uint32_t NextId;

    uint32_t mId;
    FResourceHandle<FMesh> mMesh;
    FMaterial *mMaterial;
    FTransform *mTransform;

    uint64_t mLayer = 1;
    uint32_t mRendererId = 0;

    bool bIsActive = true;
    bool bIsActiveOld = true;

    bool bIsDirty = true;

public:
    FRenderable();
    virtual ~FRenderable();

public:
    void initialize();

    void update(EActorDirtyFlags flags);

    uint32_t getId() const;

    const FResourceHandle<FMesh> &getMesh() const;
    void setMesh(const FResourceHandle<FMesh> &mesh);

    FMaterial *getMaterial() const;
    void setMaterial(FMaterial *material);

    FTransform *getTransform() const;
    void setTransform(FTransform *transform);

    void setLayer(uint64_t layer);
    uint64_t getLayer() const { return mLayer; }

    void setRendererId(uint32_t id) { mRendererId = id; }
    uint32_t getRendererId() const { return mRendererId; }

    bool isActive() const;
    void setActive(bool isActive);

    void setDirty(bool dirty = true);
    bool isDirty() const { return bIsDirty; }

    FMatrix4 getMatrix() const;
};
