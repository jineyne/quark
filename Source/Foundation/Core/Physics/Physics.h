#pragma once

#include "CorePrerequisites.h"
#include "Math/Octree.h"
#include "Misc/Module.h"
#include "PlaneCollider.h"
#include "RigidBody.h"
#include "SphereCollider.h"
#include "Physics.g.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPhysics, Debug);

QCLASS()
class DLL_EXPORT Physics : public TModule<Physics> {
    GENERATED_BODY()

public:

private:
    TArray<RigidBody *> mRegisteredRigidBodyList;
    TArray<Collider *> mRegisteredColliderList;

    QPROPERTY()
    Vector3 mGravity;

    Octree<Collider *> *mOctree;

public:
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
