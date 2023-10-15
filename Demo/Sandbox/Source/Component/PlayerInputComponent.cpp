#include "PlayerInputComponent.h"
#include "Component/CameraComponent.h"
#include "Component/FollowTargetComponent.h"
#include "Manager/InputManager.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "ShipAIComponent.h"
#include "CoreApplication.h"
#include "Misc/Time.h"

void PlayerInputComponent::onStart() {
    Component::onStart();

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

                break;

            case EKeyCode::S:

                break;

            case EKeyCode::Q:

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
                    mMainCamera = CameraComponent::GetMainComponent();//  Actor::Find(TEXT("MainCamera"));
                    mMCFollowTarget = mMainCamera->getOwner()->getComponent<FollowTargetComponent>();
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
