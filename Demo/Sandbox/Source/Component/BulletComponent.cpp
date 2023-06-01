#include "BulletComponent.h"
#include "Component/CubeColliderComponent.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"

void BulletComponent::onCreate() {
    auto collider = getOwner()->addComponent<CubeColliderComponent>();
    collider->setHalfSize(Vector3(0.1, 0.1, 0.1));

    collider->CollisionEnter.bindDynamic(BulletComponent::onCollisionEnter);
}

void BulletComponent::onUpdate() {
    mLifeTime -= gTime().getDeltaTime();

    if (mLifeTime <= 0) {
        getOwner()->destroy();
        return;
    }

    getTransform()->move(getTransform()->getForward() * 50 * gTime().getDeltaTime());
}

void BulletComponent::onCollisionEnter(Collider *other) {
    Actor *otherActor = other->getTransform()->getOwner();
    if (otherActor->getTags() != TEXT("Bullet")) {
        return;
    }

    getOwner()->destroy();
    otherActor->destroy();
}

float BulletComponent::getDamage() const {
    return mDamage;
}

void BulletComponent::setDamage(float damage) {
    mDamage = damage;
}

int BulletComponent::getTeam() const {
    return mTeam;
}

void BulletComponent::setTeam(int team) {
    mTeam = team;
}
