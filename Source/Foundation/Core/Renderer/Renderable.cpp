//
// Created by jiney on 2023-03-31.
//

#include "Renderable.h"
#include "Renderer.h"

uint32_t FRenderable::NextId = 1;

FRenderable::FRenderable() {
    mId = NextId++;
}

FRenderable::~FRenderable() {

}

void FRenderable::initialize() {
    gRenderer().notifyRenderableCreated(this);
}

void FRenderable::update(EActorDirtyFlags flags) {
    EActorDirtyFlags updateEverythingFlag = EActorDirtyFlags::Everything | EActorDirtyFlags::Active;

    if ((flags & updateEverythingFlag) != EActorDirtyFlags::None) {
        if (bIsActiveOld != bIsActive) {
            if (bIsActive)
                gRenderer().notifyRenderableCreated(this);
            else
                gRenderer().notifyRenderableRemoved(this);
        } else {
            gRenderer().notifyRenderableRemoved(this);
            gRenderer().notifyRenderableCreated(this);
        }

        bIsActiveOld = bIsActive;
        bIsDirty = false;
    } else if ((flags & EActorDirtyFlags::Mobility) != EActorDirtyFlags::None) {
        gRenderer().notifyRenderableRemoved(this);
        gRenderer().notifyRenderableCreated(this);
    } else if ((flags & EActorDirtyFlags::Transform) != EActorDirtyFlags::None) {
        if (bIsActive) {
            gRenderer().notifyRenderableUpdated(this);
        }
    }
}

uint32_t FRenderable::getId() const {
    return mId;
}

FMesh *FRenderable::getMesh() const {
    return mMesh;
}

void FRenderable::setMesh(FMesh *mesh) {
    this->mMesh = mesh;
    bIsDirty = true;
}

FMaterial *FRenderable::getMaterial() const {
    return mMaterial;
}

void FRenderable::setMaterial(FMaterial *material) {
    this->mMaterial = material;
    bIsDirty = true;
}

FTransform *FRenderable::getTransform() const {
    return mTransform;
}

void FRenderable::setTransform(FTransform *transform) {
    this->mTransform = transform;
    bIsDirty = true;
}

void FRenderable::setLayer(uint64_t layer) {
    this->mLayer = layer;
    bIsDirty = true;
}

bool FRenderable::isActive() const {
    return bIsActive;
}

void FRenderable::setActive(bool isActive) {
    this->bIsActive = isActive;
    bIsDirty = true;
}

FMatrix4 FRenderable::getMatrix() const {
    if (mTransform == nullptr) {
        return FMatrix4::Identity();
    }

    return mTransform->getWorldMatrix();
}
