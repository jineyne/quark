#pragma once

#include "Prerequisites.h"
#include "Scene/Component.h"
#include "Input/InputType.h"
#include "PlayerInputComponent.g.h"

QCLASS()
class PlayerInputComponent : public Component, public IInputEventListener {
    GENERATED_BODY()

private:
    // PlayerComponent *mPlayer;
    class RigidBodyComponent *mRigidBody;
    class ShipAIComponent *mShipAI;

    float mRotation = 0;
    float mTargetRotation = 0;

public:
    void onStart() override;

    void onActive() override;
    void onDeactive() override;

    void onUpdate() override;

    bool onInputEvent(const InputEvent &event) override;
};