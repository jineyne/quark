#pragma once

#include "CorePrerequisites.h"
#include "Physics/RigidBody.h"
#include "Scene/Component.h"
#include "RigidBodyComponent.g.h"

QCLASS()
class DLL_EXPORT RigidBodyComponent : public Component {
    GENERATED_BODY()

private:
    RigidBody *mInternal;
    Vector3 mVelocity;
    Vector3 mForce;
    float mMass;

public:
    void onCreate() override;
    void onDestroy() override;

    void onActive() override;
    void onDeactive() override;

    void addForce(Vector3 force);

    const Vector3 &getVelocity() const { return mVelocity; }
    void setVelocity(const Vector3 &velocity);

    const Vector3 &getForce() const { return mForce; }
    void setForce(const Vector3 &force);

    float getMass() const { return mMass; }
    void setMass(float mass);
};