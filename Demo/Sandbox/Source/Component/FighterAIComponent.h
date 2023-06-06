#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "ShipAIComponent.h"
#include "FighterAIComponent.g.h"

QCLASS()
class FighterAIComponent : public ShipAIComponent {
    GENERATED_BODY()
private:
    float mDetectRange = 100;
    float mBulletInterval = 0;

    Actor *mTarget;
    ShipAIComponent *mTargetAI;
    class RigidBodyComponent *mRigidBody = nullptr;

    TArray<Actor *> mDetectTargets;

    Vector3 mDestination;

public:
    void onCreate() override;
    void onStart() override;

    void onFixedUpdate() override;

    void onDetectCollisionEnter(class Collider *collider);
    void onTargetDestroyed();

protected:
    void setupAI() override;
};