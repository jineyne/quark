#include "LightComponent.h"
#include "Scene/Actor.h"

void LightComponent::onCreate() {
    mInternal = q_new<LightBase>();
    mInternal->setTransform(getOwner()->getTransform());

    mInternal->initialize();
}

void LightComponent::onStart() { }

void LightComponent::onUpdate() {
    if (mInternal->isDirty()) {
        mInternal->updateData(EActorDirtyFlags::Everything);
    }
}

void LightComponent::onActive() {
    mInternal->setType(mType);
    mInternal->setCastShadow(mCastShadow);
    mInternal->setColor(mColor);
    mInternal->setRange(mRange);
    mInternal->setSpotAngle(mSpotAngle);
    mInternal->setIntensity(mIntensity);

    mInternal->setActive(true);
    mInternal->updateData(EActorDirtyFlags::Active);
}

void LightComponent::onDeactive() {
    mInternal->setActive(false);
    mInternal->updateData(EActorDirtyFlags::Active);
}

void LightComponent::onTransformChanged(const ETransformChangedFlags &flags) {
    if ((flags & ETransformChangedFlags::Transform) == ETransformChangedFlags::Transform) {
        mInternal->updateData(EActorDirtyFlags::Transform);
    }
}

ELightType LightComponent::getType() const {
    return mType;
}

void LightComponent::setType(ELightType type) {
    mType = type;

    if (isActive()) {
        mInternal->setType(type);
    }
}

bool LightComponent::getCastShadow() const {
    return mCastShadow;
}

void LightComponent::setCastShadow(bool cast) {
    mCastShadow = cast;

    if (isActive()) {
        mInternal->setCastShadow(cast);
    }
}

Color LightComponent::getColor() const {
    return mColor;
}

void LightComponent::setColor(Color color) {
    mColor = color;

    if (isActive()) {
        mInternal->setColor(color);
    }
}

float LightComponent::getRange() const {
    return mRange;
}

void LightComponent::setRange(float range) {
    mRange = range;

    if (isActive()) {
        mInternal->setRange(range);
    }
}

Degree LightComponent::getSpotAngle() const {
    return mSpotAngle;
}

void LightComponent::setSpotAngle(Degree angle) {
    mSpotAngle = angle;

    if (isActive()) {
        mInternal->setSpotAngle(angle);
    }
}

float LightComponent::getIntensity() const {
    return mIntensity;
}

void LightComponent::setIntensity(float intensity) {
    mIntensity = intensity;

    if (isActive()) {
        mInternal->setIntensity(intensity);
    }
}
