#pragma once

#include "CorePrerequisites.h"
#include "Scene/Transform.h"

class DLL_EXPORT RigidBody {
private:
    Transform *mTransform;

    Vector3 mVelocity;
    Vector3 mForce;
    float mMass;

    bool bIsActive = true;
    bool bIsActiveOld = true;

public:
    RigidBody();
    ~RigidBody();

public:
    void initialize();
    void updateData(EActorDirtyFlags flags);

    Transform *getTransform() const;
    void setTransform(Transform *transform);

    const Vector3 &getVelocity() const;
    void setVelocity(const Vector3 &velocity);

    const Vector3 &getForce() const;
    void setForce(const Vector3 &force);

    float getMass() const;
    void setMass(float mass);
};
