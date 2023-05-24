#include "MeshRendererComponent.h"
#include "Scene/Actor.h"

void FMeshRendererComponent::onCreate() {
    mInternal = q_new<FRenderable>();
    mInternal->setTransform(getOwner()->getTransform());

    mInternal->initialize();
}

void FMeshRendererComponent::onDestroy() {
    q_delete(mInternal);
}

void FMeshRendererComponent::onStart() { }

void FMeshRendererComponent::onUpdate() {
    if (mInternal->isDirty()) {
        mInternal->update(EActorDirtyFlags::Everything);
    }
}

void FMeshRendererComponent::onActive() {
    mInternal->setMaterial(mMaterial);
    mInternal->setMesh(mMesh);

    mInternal->update(EActorDirtyFlags::Active);
}

void FMeshRendererComponent::onDeactive() {
    mInternal->update(EActorDirtyFlags::Active);
}

void FMeshRendererComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->update(EActorDirtyFlags::Transform);
    }
}

void FMeshRendererComponent::setMesh(const FResourceHandle<FMesh> &mesh) {
    mMesh = mesh;

    if (isActive()) {
        mInternal->setMesh(mesh);
    }
}

const FResourceHandle<FMesh> &FMeshRendererComponent::getMesh() const {
    return mMesh;
}

void FMeshRendererComponent::setMaterial(FMaterial *material) {
    mMaterial = material;

    if (isActive()) {
        mInternal->setMaterial(material);
    }
}

FMaterial *FMeshRendererComponent::getMaterial() const {
    return mMaterial;
}