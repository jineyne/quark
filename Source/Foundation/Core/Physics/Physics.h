#pragma once

#include "CorePrerequisites.h"
#include "RigidBody.h"
#include "Misc/Module.h"
#include "Physics.g.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPhysics, Debug);

QCLASS()
class DLL_EXPORT Physics : public TModule<Physics> {
    GENERATED_BODY()

private:
    TArray<RigidBody *> mRegisteredRigidBodyList;

    QPROPERTY()
    Vector3 mGravity;

public:
    void update();

    void notifyRigidBodyCreated(RigidBody *body);
    void notifyRigidBodyRemoved(RigidBody *body);
};

DLL_EXPORT Physics &gPhysics();
