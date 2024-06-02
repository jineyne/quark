#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "PhysicsScene.h"
#include "Physics.g.h"

struct DLL_EXPORT PhysicsDesc {
    Vector3 gravity = Vector3(0.0f, -9.81, 0.0f);
};

QCLASS(abstract)
class DLL_EXPORT Physics : public TModule<Physics> {
    GENERATED_BODY();

private:
    PhysicsDesc mDesc;

public:
    Physics(const PhysicsDesc &desc);

public:
    virtual void fixedUpdate(float dt);

    virtual PhysicsScene *createPhysicsScene() = 0;

    const PhysicsDesc &getDesc() const { return mDesc; }
};

DLL_EXPORT Physics &gPhysics();