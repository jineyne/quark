#pragma once

#include "CorePrerequisites.h"
#include "Physics/SphereCollider.h"
#include "Scene/Component.h"
#include "SphereColliderComponent.g.h"

QCLASS()
class DLL_EXPORT SphereColliderComponent : public Component {
    GENERATED_BODY();

private:
    SphereCollider *mInternal;

    QPROPERTY()
    float mRadius;

    QPROPERTY()
    Vector3 mOffset;

public:
    void onCreate() override;
    void onDestroy() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    float getRadius() const;
    void setRadius(float radius);

    const Vector3 &getOffset() const;
    void setOffset(const Vector3 &offset);
};