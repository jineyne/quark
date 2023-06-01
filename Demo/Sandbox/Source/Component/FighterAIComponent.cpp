#include "FighterAIComponent.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"
#include "Component/SphereColliderComponent.h"
#include "Component/RigidBodyComponent.h"

void FighterAIComponent::onCreate() {
    Super::onCreate();

    // detect range
    auto collider = getOwner()->addComponent<SphereColliderComponent>();
    collider->setRadius(mDetectRange);

    collider->CollisionEnter.bindDynamic(FighterAIComponent::onDetectCollisionEnter);

    // go forward when start
    // mDestination = getTransform()->getPosition() + getTransform()->getForward().normalized() * 100;
}

void FighterAIComponent::onStart() {
    Component::onStart();

    mRigidBody = getOwner()->getComponent<RigidBodyComponent>();
}

void FighterAIComponent::onUpdate() {
    Super::onUpdate();

    mBulletInterval -= gTime().getDeltaTime();

    if (mTarget != nullptr) {
        if (mBulletInterval <= 0) {
            mBulletInterval = 1;

            fire();
        }
    }

    if (mDestination.length() == 0) {
        mDestination = getTransform()->getPosition();
    }

    getTransform()->setPosition(Vector3::Lerp(getTransform()->getPosition(), mDestination, gTime().getDeltaTime()));
}

void FighterAIComponent::onFixedUpdate() {
    Super::onFixedUpdate();

    auto position = getTransform()->getPosition();

    if (position.distance(mDestination) < 1) {
        setRandomDestination();
    }

    for (int i = 0; i < mDetectTargets.length(); ++i) {
        auto target = mDetectTargets[i];
        if (Vector3::Distance(target->getTransform()->getPosition(), position) > 100) {
            mDetectTargets.removeAt(i--);
        }
    }

    if (mTarget != nullptr) {
        if (!mTarget->isDestroyed() && mTargetAI->getHealth() > 0) {
            float distance = Vector3::Distance(mTarget->getTransform()->getPosition(), position);
            if (distance > 100) {
                mTarget = nullptr;
                mTargetAI = nullptr;

                findTarget();
            } else {
                getTransform()->lookAt(mTarget->getTransform()->getPosition(), getTransform()->getUp());
                if (distance < 50){
                    mDestination = getTransform()->getPosition();
                } else {
                    mDestination = mTarget->getTransform()->getPosition();
                }
            }
        } else {
            mTarget = nullptr;
            mDestination = getTransform()->getPosition() + getTransform()->getForward().normalized() * 10;
        }
    } else {
        getTransform()->lookAt(mDestination, getTransform()->getUp());
    }

    /*if (mRigidBody) {
        mRigidBody->addForce(getTransform()->getForward().normalized() * 100);
    }*/
}


void FighterAIComponent::onDetectCollisionEnter(Collider *collider) {
    auto actor = collider->getTransform()->getOwner();

    // is ship?
    auto ai = actor->getComponent<ShipAIComponent>();
    if (ai == nullptr) {
        return;
    }

    // is in detect range?
    if (getTransform()->getPosition().distance(actor->getTransform()->getPosition()) > mDetectRange) {
        return;
    }

    // is team?
    if (ai->getTeam() == getTeam()) {
        return;
    }

    mTarget = actor;
    mTargetAI = ai;
}

void FighterAIComponent::findTarget() {
    if (mDetectTargets.empty()) {
        return;
    }

    mTarget = mDetectTargets[0];
    mTargetAI = mTarget->getComponent<ShipAIComponent>();
}

void FighterAIComponent::setRandomDestination() {

}
