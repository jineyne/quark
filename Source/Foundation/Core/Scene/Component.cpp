#include "Component.h"
#include "Manager/SceneManager.h"
#include "Actor.h"
#include "Transform.h"

void FComponent::attachTo(FActor *actor) {
    mOwnerActor = actor;
    mOwnerActor->addAndInitializeComponent(this);
}

void FComponent::detachFrom(FActor *actor) {
    if (mOwnerActor != actor) {
        return;
    }

    mOwnerActor->removeComponent(this);
    mOwnerActor = nullptr;
}

void FComponent::destroy(bool immediate) {
    mOwnerActor->destroyComponent(this, immediate);
    // FSceneObject::destroy(immediate);
}

void FComponent::setActive(bool active) {
    if (mActiveSelf != active) {
        if (active) {
            gSceneManager().notifyComponentActivated(this);
        } else {
            gSceneManager().notifyComponentDeactivated(this);
        }
    }

    FSceneObject::setActive(active);
}

FTransform *FComponent::getTransform() const {
    return getOwner()->getTransform();
}

void FComponent::destroyInternal(bool immediate) {
    FSceneObject::destroyInternal(immediate);

    if (immediate) {
        gSceneManager().notifyComponentDestroyed(this, immediate);
    }
}
