#pragma once

#include "B2DPrerequisites.h"

#include <Physics/SphereCollider2D.h>

class B2D_EXPORT B2DSphereCollider2D : public SphereCollider2D {
private:
    B2DPhysics *mPhysics;
    B2DPhysicsScene *mScene;

    float mRadius;

    b2Body *mBody;
    b2Fixture *mFixture;

public:
    B2DSphereCollider2D(B2DPhysics *physics, B2DPhysicsScene *scene, float radius, Transform *transform);
    ~B2DSphereCollider2D();

public:
    void setRadius(float radius) override;

    void setPhysicsBodyType(EPhysicsBodyType type) override;

    void setIsTrigger(bool isTrigger) override;

    void setOffset(Vector2 offset) override;

    void updateTransform(Vector2 position, FQuaternion rotation) override;

    Vector2 getPosition() const override;

    FQuaternion getRotation() const override;
};