#include "MeshRendererComponent.h"
#include "Scene/Actor.h"

void MeshRendererComponent::onCreate() {
    mInternal = q_new<Renderable>();
    mInternal->setTransform(getOwner()->getTransform());

    mInternal->initialize();
}

void MeshRendererComponent::onDestroy() {
    q_delete(mInternal);
}

void MeshRendererComponent::onStart() { }

void MeshRendererComponent::onUpdate() {
    if (mInternal->isDirty()) {
        mInternal->updateData(EActorDirtyFlags::Everything);
    }
}

void MeshRendererComponent::onActive() {
    mInternal->setMaterial(mMaterial);
    mInternal->setMesh(mMesh);

    mInternal->setActive(true);
    mInternal->updateData(EActorDirtyFlags::Active);
}

void MeshRendererComponent::onDeactive() {
    if (!isDestroyed()) {
        mInternal->setActive(false);
        mInternal->updateData(EActorDirtyFlags::Active);
    }
}

void MeshRendererComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->updateData(EActorDirtyFlags::Transform);
    }
}

void MeshRendererComponent::setMesh(const FResourceHandle<Mesh> &mesh) {
    mMesh = mesh;

    if (isActive()) {
        mInternal->setMesh(mesh);
    }
}

const FResourceHandle<Mesh> &MeshRendererComponent::getMesh() const {
    return mMesh;
}

void MeshRendererComponent::setMaterial(Material *material) {
    mMaterial = material;

    if (isActive()) {
        mInternal->setMaterial(material);
    }
}

Material *MeshRendererComponent::getMaterial() const {
    return mMaterial;
}