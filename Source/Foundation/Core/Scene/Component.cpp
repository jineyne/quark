#include "Component.h"
#include "Manager/SceneManager.h"
#include "Actor.h"
#include "Transform.h"
#include "Manager/SceneObjectManager.h"

void Component::attachTo(Actor *actor) {
    mOwnerActor = actor;
    mOwnerActor->addAndInitializeComponent(this);
}

void Component::detachFrom(Actor *actor) {
    if (mOwnerActor != actor) {
        return;
    }

    mOwnerActor->removeComponent(this);
    mOwnerActor = nullptr;
}

void Component::destroy(bool immediate) {
    mOwnerActor->destroyComponent(this, immediate);
}

void Component::setActive(bool active) {
    if (mActiveSelf != active) {
        if (active) {
            gSceneManager().notifyComponentActivated(this);
        } else {
            gSceneManager().notifyComponentDeactivated(this);
        }
    }

    SceneObject::setActive(active);
}

Transform *Component::getTransform() const {
    return getOwner()->getTransform();
}

void Component::destroyInternal(bool immediate) {
    if (immediate) {
        gSceneObjectManager().unregisterObject(this);
    } else {
        gSceneObjectManager().queueForDestroy(this);
    }
}
