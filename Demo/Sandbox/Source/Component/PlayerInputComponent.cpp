#include "PlayerInputComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/FollowTargetComponent.h"
#include "Manager/InputManager.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "ShipAIComponent.h"
#include "CoreApplication.h"
#include "Misc/Time.h"

void PlayerInputComponent::onStart() {
    Component::onStart();

    mRigidBody = getOwner()->getComponent<RigidBodyComponent>();
    mShipAI = getOwner()->getComponent<ShipAIComponent>();
}

void PlayerInputComponent::onActive() {
    gInputManager().addEventListener(this);
}

void PlayerInputComponent::onDeactive() {
    gInputManager().removeEventListener(this);
}

bool PlayerInputComponent::onInputEvent(const InputEvent &event) {
    if (event.state == EInputState::Pressed || event.state == EInputState::Changed) {
        switch (event.keyCode) {
            case EKeyCode::W:
                if (mRigidBody) {
                    mRigidBody->addForce(getTransform()->getForward().normalized() * 100);
                }
                break;

            case EKeyCode::S:
                if (mRigidBody) {
                    mRigidBody->addForce(getTransform()->getForward().normalized() * -100);
                }
                break;

            case EKeyCode::Q:
                if (mRigidBody) {
                    mRigidBody->setVelocity(Vector3::ZeroVector);
                }
                break;

            case EKeyCode::Space:
                if (mShipAI) {
                    mShipAI->fire();
                }
                break;

            case EKeyCode::MouseWheelUp:
            case EKeyCode::MouseWheelDown:
                LOG(LogTemp, Debug, TEXT("Delta: %lf"), event.value);
                if (mMainCamera == nullptr) {
                    mMainCamera = Actor::Find(TEXT("MainCamera"));
                    mMCFollowTarget = mMainCamera->getComponent<FollowTargetComponent>();
                }

                // mMainCamera->getTransform()->move(Vector3(0, 0, event.value));
                mMCFollowTarget->setOffset(mMCFollowTarget->getOffset() - Vector3(0, 0, event.value));

                break;
        }
    }

    switch (event.keyCode) {
        case EKeyCode::A:
            mTargetRotation -= 100 * gTime().getDeltaTime();
            getTransform()->rotate(FQuaternion(mTargetRotation, 0, 0));
            break;

        case EKeyCode::D:
            mTargetRotation += 100 * gTime().getDeltaTime();
            getTransform()->rotate(FQuaternion(mTargetRotation, 0, 0));
            break;
    }

    /*if (event.keyCode == EKeyCode::C) {
        gCoreApplication().quitRequest();
    }*/

    return false;
}
