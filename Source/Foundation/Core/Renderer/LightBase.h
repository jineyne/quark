#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"
#include "Scene/Transform.h"

enum class ELightType {
    Point = 0,
    Spot = 1,
    Directional = 2,
};

class DLL_EXPORT FLightBase {
public:
    bool bInitialized = false;

    FTransform *mTransform;

    ELightType mType;
    bool mCastShadow;
    FColor mColor;
    float mRange;
    FDegree mSpotAngle;
    float mIntensity;

    uint32_t mRendererId = 0;

public:
    virtual ~FLightBase();

public:
    void initialize();
    void update(EActorDirtyFlags flags);

    void setTransform(FTransform *transform);
    FTransform *getTransform() const { return mTransform; }

    ELightType getType() const { return mType; }
    void setType(ELightType type);

    bool getCastShadow() const { return mCastShadow; }
    void setCastShadow(bool cast);

    FColor getColor() const { return mColor; }
    void setColor(FColor color) { mColor = color; }

    float getRange() const { return mRange; }
    void setRange(float range);

    FDegree getSpotAngle() const { return mSpotAngle; }
    void setSpotAngle(FDegree angle);

    float getIntensity() const { return mIntensity; }
    void setIntensity(float intensity);

    void setRendererId(uint32_t id) { mRendererId = id; }
    uint32_t getRendererId() const { return mRendererId; }
};