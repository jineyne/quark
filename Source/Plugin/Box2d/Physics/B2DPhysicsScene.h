#pragma once

#include "B2DPrerequisites.h"

#include <Physics/PhysicsScene.h>
#include <Physics/Physics.h>

class B2D_EXPORT B2DPhysicsScene : public PhysicsScene, b2ContactListener {
private:
    B2DPhysics *mPhysics;
    b2World *mWorld = nullptr;

public:
    B2DPhysicsScene(B2DPhysics *physics, const PhysicsDesc &desc);
    ~B2DPhysicsScene();

public:
    BoxCollider2D *createBoxCollider2D(const Size &size, Transform *transform) override;
    SphereCollider2D *createSphereCollider2D(float radius, Transform *transform) override;

private:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

public:

    b2World *getWorld() const { return mWorld; }
};

