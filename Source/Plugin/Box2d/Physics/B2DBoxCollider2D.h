#pragma once

#include "B2DPrerequisites.h"

#include <Physics/BoxCollider2D.h>

class B2D_EXPORT B2DBoxCollider2D : public BoxCollider2D {
private:
    B2DPhysics *mPhysics;
    B2DPhysicsScene *mScene;

    Size mSize;

    b2Body *mBody;
    b2Fixture *mFixture;

public:
    B2DBoxCollider2D(B2DPhysics *physics, B2DPhysicsScene *scene, const Size &size, Transform *transform);
    ~B2DBoxCollider2D();

public:
    void setPhysicsBodyType(EPhysicsBodyType type) override;

    void setIsTrigger(bool isTrigger) override;
    void setSize(Size size) override;
    void setOffset(Vector2 offset) override;

    void updateTransform(Vector2 position, FQuaternion rotation) override;

    Vector2 getPosition() const override;

    FQuaternion getRotation() const override;

    b2Body *getBody() const;
    b2Fixture *getFixture() const;
};