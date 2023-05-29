#pragma once

#include "CorePrerequisites.h"
#include "Physics/CubeCollider.h"
#include "Scene/Component.h"
#include "CubeColliderComponent.g.h"

QCLASS()
class DLL_EXPORT CubeColliderComponent : public Component {
    GENERATED_BODY();

public:
    TEvent<void(Collider *)> CollisionEnter;
    TEvent<void(Collider *)> CollisionStay;
    TEvent<void(Collider *)> CollisionExit;

private:
    CubeCollider *mInternal;

    QPROPERTY()
    Vector3 mHalfSize;

    QPROPERTY()
    Vector3 mOffset;

public:
    void onCreate() override;
    void onDestroy() override;

    void onActive() override;
    void onDeactive() override;

    void onTransformChanged(const ETransformChangedFlags &flags) override;

    const Vector3 &getHalfSize() const;
    void setHalfSize(const Vector3 &halfSize);

    const Vector3 &getOffset() const;
    void setOffset(const Vector3 &offset);

private:
    void onCollisionEnter(Collider *other);
    void onCollisionStay(Collider *other);
    void onCollisionExit(Collider *other);
};