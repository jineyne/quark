#include "Transform.h"

FTransform::FTransform(FActor *owner) : mOwner(owner) {

}

FTransform *FTransform::New(FActor *owner) {
    return q_new<FTransform>(owner);
}

void FTransform::move(const FVector3 &val) {
    /*if (mOwner) {
        mOwner->notifyTransformChanged(TransformChangedFlagBit::Transform);
    }*/

    mPosition += val;
}

void FTransform::rotate(const FQuaternion &val) {
    /*if (mOwner) {
        mOwner->notifyTransformChanged(TransformChangedFlagBit::Transform);
    }*/

    mRotation += val;
}

void FTransform::scale(const FVector3 &val) {
    /*if (mOwner) {
        mOwner->notifyTransformChanged(TransformChangedFlagBit::Transform);
    }*/

    mScale += val;
}

void FTransform::setParent(FTransform *parent) {
    /*if (mOwner) {
        mOwner->notifyTransformChanged(TransformChangedFlagBit::Transform);
    }*/

    mParent = parent;
}

void FTransform::setPosition(const FVector3 &pos) {
    /*if (mOwner) {
        mOwner->notifyTransformChanged(TransformChangedFlagBit::Transform);
    }*/

    mPosition = pos;
}

void FTransform::setRotation(const FQuaternion &rot) {
    /*if (mOwner) {
        mOwner->notifyTransformChanged(TransformChangedFlagBit::Transform);
    }*/

    mRotation = rot;
}

void FTransform::setScale(const FVector3 &scale) {
    /*if (mOwner) {
        mOwner->notifyTransformChanged(TransformChangedFlagBit::Transform);
    }*/

    mScale = scale;
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

    /*mCachedLocMat = FMatrix4(1.0f);
    mCachedLocMat.scale(mScale);
    mCachedLocMat.rotate(mRotation);
    mCachedLocMat.translate(mPosition);*/

    mCachedLocMat = FMatrix4::Translate(mPosition) * FMatrix4::Rotate(mRotation) * FMatrix4::Scale(mScale);

    if (mParent != nullptr) {
        auto parentMatrix = mParent->getWorldMatrix();
        mCachedWorMat = parentMatrix * mCachedLocMat;
    } else {
        mCachedWorMat = mCachedLocMat;
    }
}
