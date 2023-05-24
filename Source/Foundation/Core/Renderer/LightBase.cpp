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
    EActorDirtyFlags updateEverythingFlag = EActorDirtyFlags::Everything | EActorDirtyFlags::Active;

    if ((flags & updateEverythingFlag) != EActorDirtyFlags::None) {
        if (bIsActiveOld != bIsActive) {
            if (bIsActive)
                gRenderer().notifyLightCreated(this);
            else
                gRenderer().notifyLightRemoved(this);
        } else {
            gRenderer().notifyLightRemoved(this);
            gRenderer().notifyLightCreated(this);
        }

        bIsActiveOld = bIsActive;
        bIsDirty = false;
    } else if ((flags & EActorDirtyFlags::Mobility) != EActorDirtyFlags::None) {
        gRenderer().notifyLightRemoved(this);
        gRenderer().notifyLightCreated(this);
    } else if ((flags & EActorDirtyFlags::Transform) != EActorDirtyFlags::None) {
        if (bIsActive) {
            gRenderer().notifyLightUpdated(this);
        }
    }
}

void FLightBase::setTransform(FTransform *transform) {
    mTransform = transform;
    bIsDirty = true;
}

void FLightBase::setType(ELightType type) {
    mType = type;
    bIsDirty = true;
}

void FLightBase::setCastShadow(bool cast) {
    mCastShadow = cast;
    bIsDirty = true;
}

void FLightBase::setRange(float range) {
    mRange = range;
    bIsDirty = true;
}

void FLightBase::setSpotAngle(FDegree angle) {
    mSpotAngle = angle;
    bIsDirty = true;
}

void FLightBase::setIntensity(float intensity) {
    mIntensity = intensity;
    bIsDirty = true;
}

void FLightBase::setActive(bool isActive) {
    bIsActive = isActive;
    bIsDirty = true;
}
