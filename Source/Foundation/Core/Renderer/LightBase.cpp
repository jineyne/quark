#include "LightBase.h"
#include "Renderer.h"

LightBase::~LightBase() {
    gRenderer().notifyLightRemoved(this);
}

void LightBase::initialize() {
    if (bInitialized) {
        return;
    }

    bInitialized = true;

    gRenderer().notifyLightCreated(this);
}

void LightBase::update(EActorDirtyFlags flags) {
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

void LightBase::setTransform(Transform *transform) {
    mTransform = transform;
    bIsDirty = true;
}

void LightBase::setType(ELightType type) {
    mType = type;
    bIsDirty = true;
}

void LightBase::setCastShadow(bool cast) {
    mCastShadow = cast;
    bIsDirty = true;
}

void LightBase::setRange(float range) {
    mRange = range;
    bIsDirty = true;
}

void LightBase::setSpotAngle(Degree angle) {
    mSpotAngle = angle;
    bIsDirty = true;
}

void LightBase::setIntensity(float intensity) {
    mIntensity = intensity;
    bIsDirty = true;
}

void LightBase::setActive(bool isActive) {
    bIsActive = isActive;
    bIsDirty = true;
}
