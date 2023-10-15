#pragma once

#include "CorePrerequisites.h"
#include "Renderer/LightBase.h"
#include "Scene/Component.h"
#include "LightComponent.g.h"

QCLASS()
class DLL_EXPORT LightComponent : public Component {
    GENERATED_BODY()
private:
    LightBase *mInternal;

    QPROPERTY()
    ELightType mType;

    QPROPERTY()
    bool mCastShadow;

    QPROPERTY()
    Color mColor;

    QPROPERTY()
    float mRange;

    QPROPERTY()
    Degree mSpotAngle;

    QPROPERTY()
    float mIntensity;

public:
    void onCreate() override;

    void onDestroy() override;

    void onStart() override;
    void onUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    ELightType getType() const;
    void setType(ELightType type);

    bool getCastShadow() const;
    void setCastShadow(bool cast);

    Color getColor() const;
    void setColor(Color color);

    float getRange() const;
    void setRange(float range);

    Degree getSpotAngle() const;
    void setSpotAngle(Degree angle);

    float getIntensity() const;
    void setIntensity(float intensity);
};
