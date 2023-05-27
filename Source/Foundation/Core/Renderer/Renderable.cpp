#include "Renderable.h"
#include "Renderer.h"

uint32_t Renderable::NextId = 1;

Renderable::Renderable() {
    mId = NextId++;
}

Renderable::~Renderable() {
    gRenderer().notifyRenderableRemoved(this);
}

void Renderable::initialize() {
    gRenderer().notifyRenderableCreated(this);
}

void Renderable::update(EActorDirtyFlags flags) {
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

uint32_t Renderable::getId() const {
    return mId;
}

const FResourceHandle<Mesh> &Renderable::getMesh() const {
    return mMesh;
}

void Renderable::setMesh(const FResourceHandle<Mesh> &mesh) {
    this->mMesh = mesh;
    bIsDirty = true;
}

Material *Renderable::getMaterial() const {
    return mMaterial;
}

void Renderable::setMaterial(Material *material) {
    this->mMaterial = material;
    bIsDirty = true;
}

Transform *Renderable::getTransform() const {
    return mTransform;
}

void Renderable::setTransform(Transform *transform) {
    this->mTransform = transform;
    bIsDirty = true;
}

void Renderable::setLayer(uint64_t layer) {
    this->mLayer = layer;
    bIsDirty = true;
}

bool Renderable::isActive() const {
    return bIsActive;
}

void Renderable::setActive(bool isActive) {
    this->bIsActive = isActive;
    bIsDirty = true;
}

void Renderable::setDirty(bool dirty) {
    bIsDirty = dirty;
}

Matrix4 Renderable::getMatrix() const {
    if (mTransform == nullptr) {
        return Matrix4::Matrix4();
    }

    return mTransform->getWorldMatrix();
}
