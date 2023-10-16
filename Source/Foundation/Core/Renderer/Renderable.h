#pragma once

#include "CorePrerequisites.h"
#include "Mesh/Mesh.h"
#include "Material/Material.h"
#include "Scene/Transform.h"

class DLL_EXPORT Renderable {
private:
    static uint32_t NextId;

    uint32_t mId;
    Mesh *mMesh;
    Material *mMaterial;
    Transform *mTransform;

    uint64_t mLayer = 1;
    uint32_t mRendererId = 0;

    bool bIsActive = true;
    bool bIsActiveOld = true;

    bool bIsDirty = true;

public:
    Renderable();
    virtual ~Renderable();

public:
    void initialize();

    void updateData(EActorDirtyFlags flags);

    uint32_t getId() const;

    Mesh *getMesh() const;
    void setMesh(Mesh *mesh);

    Material *getMaterial() const;
    void setMaterial(Material *material);

    Transform *getTransform() const;
    void setTransform(Transform *transform);

    void setLayer(uint64_t layer);
    uint64_t getLayer() const { return mLayer; }

    void setRendererId(uint32_t id) { mRendererId = id; }
    uint32_t getRendererId() const { return mRendererId; }

    bool isActive() const;
    void setActive(bool isActive);

    void setDirty(bool dirty = true);
    bool isDirty() const { return bIsDirty; }

    Matrix4 getMatrix() const;
};
