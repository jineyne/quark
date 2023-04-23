#include "LightBase.h"
#include "Renderer.h"

FLightBase::~FLightBase() {
    gRenderer().notifyLightRemoved(this);
}

void FLightBase::initialize() {
    if (bInitialized) {
        return;
    }

    bInitialized = true;

    gRenderer().notifyLightCreated(this);
}

void FLightBase::update(EActorDirtyFlags flags) {
    if ((flags != EActorDirtyFlags::None)) {
        gRenderer().notifyLightUpdated(this);
    }
}

void FLightBase::setTransform(FTransform *transform) {
    mTransform = transform;
}

void FLightBase::setType(ELightType type) {
    mType = type;
}

void FLightBase::setCastShadow(bool cast) {
    mCastShadow = cast;
}

void FLightBase::setRange(float range) {
    mRange = range;
}

void FLightBase::setSpotAngle(FDegree angle) {
    mSpotAngle = angle;
}

void FLightBase::setIntensity(float intensity) {
    mIntensity = intensity;
}