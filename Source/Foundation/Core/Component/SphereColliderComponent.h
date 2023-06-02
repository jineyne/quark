#pragma once

#include "CorePrerequisites.h"
#include "Physics/SphereCollider.h"
#include "Scene/Component.h"
#include "SphereColliderComponent.g.h"

QCLASS()
class DLL_EXPORT SphereColliderComponent : public Component {
    GENERATED_BODY();

public:
    TEvent<void(Collider *)> CollisionEnter;
    TEvent<void(Collider *)> CollisionStay;
    TEvent<void(Collider *)> CollisionExit;

private:
    SphereCollider *mInternal;

    QPROPERTY()
    float mRadius = 1;

    QPROPERTY()
    Vector3 mOffset;

    QPROPERTY()
    bool mIsTrigger;

public:
    void onCreate() override;
    void onDestroy() override;

    void onFixedUpdate() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    float getRadius() const;
    void setRadius(float radius);

    const Vector3 &getOffset() const;
    void setOffset(const Vector3 &offset);

    bool isTrigger() const;
    void setIsTrigger(bool isTrigger);

private:
    void onCollisionEnter(Collider *other);
    void onCollisionStay(Collider *other);
    void onCollisionExit(Collider *other);
};