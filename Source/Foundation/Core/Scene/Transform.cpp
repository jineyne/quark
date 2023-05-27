#include "Transform.h"
#include "Actor.h"

Transform::Transform(Actor *owner) : mOwner(owner) {

}

Transform *Transform::New(Actor *owner) {
    return q_new<Transform>(owner);
}

void Transform::move(const Vector3 &val) {
    mPosition += val;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void Transform::rotate(const FQuaternion &val) {
    mRotation += val;
    mRotation.normalize();

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void Transform::scale(const Vector3 &val) {
    mScale += val;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void Transform::lookAt(const Vector3 &location, const Vector3 &up) {
    Vector3 forward = location - getPosition();
    FQuaternion rotation = getRotation();
    rotation.lookRotation(forward, up);
    setRotation(rotation);
}

void Transform::setParent(Transform *parent) {
    mParent = parent;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void Transform::setPosition(const Vector3 &pos) {
    mPosition = pos;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void Transform::setRotation(const FQuaternion &rot) {
    mRotation = rot;
    mRotation.normalize();

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void Transform::setScale(const Vector3 &scale) {
    mScale = scale;

    if (mOwner) {
        mOwner->notifyTransformChanged(ETransformChangedFlags::Transform);
    }

    setDirty();
}

void Transform::setDirty(bool dirty) {
    mIsDirty = dirty;
}

const Matrix4 &Transform::getLocalMatrix() {
    updateMatrix();

    return mCachedLocMat;
}

const Matrix4 &Transform::getWorldMatrix() {
    updateMatrix();

    return mCachedWorMat;
}

void Transform::setOwner(Actor *actor) {
    mOwner = actor;
}

void Transform::updateMatrix() {
    if (!mIsDirty) {
        return;
    }

    mIsDirty = false;

    mCachedLocMat = Matrix4::Transform(mPosition, mRotation.normalized(), mScale);

    if (mParent != nullptr) {
        auto parentMatrix = mParent->getWorldMatrix();
        mCachedWorMat = parentMatrix * mCachedLocMat;
    } else {
        mCachedWorMat = mCachedLocMat;
    }
}

Vector3 Transform::getForward() const {
    return getRotation().rotate(Vector3::Forward);
}

Vector3 Transform::getRight() const {
    return getRotation().rotate(Vector3::Right);
}

Vector3 Transform::getUp() const {
    return getRotation().rotate(Vector3::Up);
}
