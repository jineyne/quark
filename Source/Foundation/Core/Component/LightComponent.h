#pragma once

#include "CorePrerequisites.h"
#include "Renderer/LightBase.h"
#include "Scene/Component.h"

class DLL_EXPORT FLightComponent : public FComponent {
private:
    FLightBase *mInternal;

    ELightType mType;
    bool mCastShadow;
    FColor mColor;
    float mRange;
    FDegree mSpotAngle;
    float mIntensity;

public:
    void onCreate() override;

    void onStart() override;
    void onUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    ELightType getType() const;
    void setType(ELightType type);

    bool getCastShadow() const;
    void setCastShadow(bool cast);

    FColor getColor() const;
    void setColor(FColor color);

    float getRange() const;
    void setRange(float range);

    FDegree getSpotAngle() const;
    void setSpotAngle(FDegree angle);

    float getIntensity() const;
    void setIntensity(float intensity);
};
