#include "LightComponent.h"
#include "Scene/Actor.h"

void FLightComponent::onCreate() {
    mInternal = q_new<FLightBase>();
    mInternal->setTransform(getOwner()->getTransform());

    mInternal->initialize();
}

void FLightComponent::onStart() { }

void FLightComponent::onUpdate() {
    if (mInternal->isDirty()) {
        mInternal->update(EActorDirtyFlags::Everything);
    }
}

void FLightComponent::onActive() {
    mInternal->setType(mType);
    mInternal->setCastShadow(mCastShadow);
    mInternal->setColor(mColor);
    mInternal->setRange(mRange);
    mInternal->setSpotAngle(mSpotAngle);
    mInternal->setIntensity(mIntensity);

    mInternal->update(EActorDirtyFlags::Active);
}

void FLightComponent::onDeactive() {
    mInternal->update(EActorDirtyFlags::Active);
}

void FLightComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->update(EActorDirtyFlags::Transform);
    }
}

ELightType FLightComponent::getType() const {
    return mType;
}

void FLightComponent::setType(ELightType type) {
    mType = type;

    if (isActive()) {
        mInternal->setType(type);
    }
}

bool FLightComponent::getCastShadow() const {
    return mCastShadow;
}

void FLightComponent::setCastShadow(bool cast) {
    mCastShadow = cast;

    if (isActive()) {
        mInternal->setCastShadow(cast);
    }
}

FColor FLightComponent::getColor() const {
    return mColor;
}

void FLightComponent::setColor(FColor color) {
    mColor = color;

    if (isActive()) {
        mInternal->setColor(color);
    }
}

float FLightComponent::getRange() const {
    return mRange;
}

void FLightComponent::setRange(float range) {
    mRange = range;

    if (isActive()) {
        mInternal->setRange(range);
    }
}

FDegree FLightComponent::getSpotAngle() const {
    return mSpotAngle;
}

void FLightComponent::setSpotAngle(FDegree angle) {
    mSpotAngle = angle;

    if (isActive()) {
        mInternal->setSpotAngle(angle);
    }
}

float FLightComponent::getIntensity() const {
    return mIntensity;
}

void FLightComponent::setIntensity(float intensity) {
    mIntensity = intensity;

    if (isActive()) {
        mInternal->setIntensity(intensity);
    }
}
