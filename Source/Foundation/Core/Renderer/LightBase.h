#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"
#include "Scene/Transform.h"

QENUM()
enum class ELightType {
    Point = 0       QENTRY(),
    Spot = 1        QENTRY(),
    Directional = 2 QENTRY(),
};

class DLL_EXPORT LightBase {
public:
    bool bInitialized = false;

    Transform *mTransform;

    ELightType mType;
    bool mCastShadow;
    Color mColor;
    float mRange;
    Degree mSpotAngle;
    float mIntensity;

    uint32_t mRendererId = 0;

    bool bIsActive = true;
    bool bIsActiveOld = true;

    bool bIsDirty = false;

public:
    virtual ~LightBase();

public:
    void initialize();
    void update(EActorDirtyFlags flags);

    void setTransform(Transform *transform);
    Transform *getTransform() const { return mTransform; }

    ELightType getType() const { return mType; }
    void setType(ELightType type);

    bool getCastShadow() const { return mCastShadow; }
    void setCastShadow(bool cast);

    Color getColor() const { return mColor; }
    void setColor(Color color) { mColor = color; }

    float getRange() const { return mRange; }
    void setRange(float range);

    Degree getSpotAngle() const { return mSpotAngle; }
    void setSpotAngle(Degree angle);

    float getIntensity() const { return mIntensity; }
    void setIntensity(float intensity);

    bool isActive() const { return bIsActive; }
    void setActive(bool isActive);

    void setRendererId(uint32_t id) { mRendererId = id; }
    uint32_t getRendererId() const { return mRendererId; }

    bool isDirty() const { return bIsDirty; }
};