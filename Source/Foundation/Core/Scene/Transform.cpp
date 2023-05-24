#include "Transform.h"
#include "Actor.h"

FTransform::FTransform(FActor *owner) : mOwner(owner) {

}

FTransform *FTransform::New(FActor *owner) {
    return q_new<FTransform>(owner);
}

void FTransform::move(const FVector3 &val) {
    mPosition += val;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void FTransform::rotate(const FQuaternion &val) {
    mRotation += val;
    mRotation.normalize();

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void FTransform::scale(const FVector3 &val) {
    mScale += val;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void FTransform::lookAt(const FVector3 &location, const FVector3 &up) {
    FVector3 forward = location - getPosition();
    FQuaternion rotation = getRotation();
    rotation.lookRotation(forward, up);
    setRotation(rotation);
}

void FTransform::setParent(FTransform *parent) {
    mParent = parent;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void FTransform::setPosition(const FVector3 &pos) {
    mPosition = pos;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void FTransform::setRotation(const FQuaternion &rot) {
    mRotation = rot;
    mRotation.normalize();

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void FTransform::setScale(const FVector3 &scale) {
    mScale = scale;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void FTransform::setDirty(bool dirty) {
    mIsDirty = dirty;
}

const FMatrix4 &FTransform::getLocalMatrix() {
    updateMatrix();

    return mCachedLocMat;
}

const FMatrix4 &FTransform::getWorldMatrix() {
    updateMatrix();

    return mCachedWorMat;
}

void FTransform::setOwner(FActor *actor) {
    mOwner = actor;
}

void FTransform::updateMatrix() {
    if (!mIsDirty) {
        return;
    }

    mIsDirty = false;

    mCachedLocMat = FMatrix4::Transform(mPosition, mRotation.normalized(), mScale);

    if (mParent != nullptr) {
        auto parentMatrix = mParent->getWorldMatrix();
        mCachedWorMat = parentMatrix * mCachedLocMat;
    } else {
        mCachedWorMat = mCachedLocMat;
    }
}

FVector3 FTransform::getForward() const {
    return getRotation().rotate(FVector3::Forward);
}

FVector3 FTransform::getRight() const {
    return getRotation().rotate(FVector3::Right);
}

FVector3 FTransform::getUp() const {
    return getRotation().rotate(FVector3::Up);
}
