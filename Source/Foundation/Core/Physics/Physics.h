#pragma once

#include "CorePrerequisites.h"
#include "Misc/Module.h"
#include "PlaneCollider.h"
#include "RigidBody.h"
#include "SphereCollider.h"
#include "AABBTree.h"
#include "Physics.g.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPhysics, Debug);

QCLASS()
class DLL_EXPORT Physics : public TModule<Physics> {
    GENERATED_BODY()

private:
    TArray<RigidBody *> mRegisteredRigidBodyList;
    TArray<Collider *> mRegisteredColliderList;

    TArray<Collider *> mTriggerColliderList;

    QPROPERTY()
    Vector3 mGravity;

    AABBTree mTree;

protected:
    void onStartUp() override;

    void onShutDown() override;

public:
    void fixedUpdate(float st);

    void notifyRigidBodyCreated(RigidBody *body);
    void notifyRigidBodyRemoved(RigidBody *body);

    void notifyColliderCreated(Collider *collider);
    void notifyColliderUpdated(Collider *collider);
    void notifyColliderRemoved(Collider *collider);
};

DLL_EXPORT Physics &gPhysics();
