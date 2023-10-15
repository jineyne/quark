#pragma once

#include "B2DPrerequisites.h"
#include "Physics/Physics.h"

#include "B2DPhysicsScene.h"

class B2D_EXPORT B2DPhysics : public Physics {
    TArray<B2DPhysicsScene *> mScenes;

public:
    B2DPhysics(const PhysicsDesc &desc);

public:
    void fixedUpdate(float dt) override;

    PhysicsScene *createPhysicsScene() override;
};
