#include "BulletComponent.h"
#include "Scene/Actor.h"
#include "Scene/Transform.h"
#include "Misc/Time.h"

void BulletComponent::onCreate() {
}

void BulletComponent::onUpdate() {
    mLifeTime -= gTime().getDeltaTime();

    if (mLifeTime <= 0) {
        getOwner()->destroy();
        return;
    }

    getTransform()->move(getTransform()->getForward() * 50 * gTime().getDeltaTime());
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
