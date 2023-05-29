#include "BulletComponent.h"
#include "Scene/Actor.h"
#include "Misc/Time.h"

void BulletComponent::onUpdate() {
    mLifeTime -= gTime().getDeltaTime();

    if (mLifeTime <= 0) {
        getOwner()->destroy();
        return;
    }

    getTransform()->move(getTransform()->getForward() * 5 * gTime().getDeltaTime());
}
