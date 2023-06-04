#include "Actor.h"
#include "Manager/SceneObjectManager.h"
#include "Manager/SceneManager.h"
#include "Reflection/ObjectHash.h"

Actor::Actor() {
    mTransform = Transform::New(this);
}

Actor *Actor::New(const String &actorName) {
    Actor *actor = newObject<Actor>(nullptr, Actor::StaticClass(), actorName);

    gSceneObjectManager().registerObject(actor);
    gSceneManager().createNewActor(actor);

    return actor;
}

Actor *Actor::Find(const String &actorName) {
    auto object = gObjectHash().find(actorName, nullptr);

    if (object->isA<Actor>()) {
        return (Actor *) object;
    }

    return nullptr;
}

void Actor::destroy(bool immediate) {
    if (mParentActor != nullptr) {
        if (!mParentActor->isDestroyed()) {
            detachFrom(mParentActor);
        }

        mParentActor = nullptr;
    }

    SceneObject::destroy(immediate);
}

void Actor::attachTo(Actor *actor) {
    if (mParentActor != nullptr) {
        detachFrom(mParentActor);
    }

    mParentActor = actor;
    if (actor->mAttachedActorList.contains(this)) {
        return;
    }

    actor->mAttachedActorList.add(this);
}

void Actor::detachFrom(Actor *actor) {
    if (mParentActor != actor) {
        return;
    }

    if (actor->mAttachedActorList.empty()) {
        return;
    }

    if (actor->mAttachedActorList.contains(this)) {
        return;
    }

    actor->mAttachedActorList.remove(this);

    mParentActor = nullptr;
}

void Actor::move(const Vector3 &value) {
    auto pos = mTransform->getPosition();
    mTransform->setPosition(pos + value);
}

void Actor::removeComponent(Component *component) {
    mAttachedComponent.remove(component);
}

void Actor::destroyComponent(Component *component, bool immediate) {
    if (component == nullptr) {
        LOG(LogScene, Warning, TEXT("Trying to remove a null component"));
    }

    if (mAttachedComponent.contains(component)) {
        if (isInitialized()) {
            gSceneManager().notifyComponentDestroyed(component, immediate);
        }

        component->destroyInternal(immediate);
        mAttachedComponent.remove(component);
    }
}

void Actor::setActive(bool active) {
    if (mActiveSelf != active) {
        for (auto &child : mAttachedActorList) {
            child->setActive(active);
        }

        for (auto &component : mAttachedComponent) {
            component->setActive(active);
        }
    }

    SceneObject::setActive(active);
}

void Actor::setScene(Scene *targetScene) {
    mTargetScene = targetScene;
}

void Actor::notifyTransformChanged(ETransformChangedFlags flags) const {
    ETransformChangedFlags componentFlags = flags;
    mTransform->setDirty();

    if (componentFlags != ETransformChangedFlags::None) {
        for (auto &entry : mAttachedComponent) {
            entry->onTransformChanged(componentFlags);
        }
    }

    flags &= ETransformChangedFlags::Mobility;
    if (flags == ETransformChangedFlags::None) {
        for (auto &entry : mAttachedActorList) {
            entry->notifyTransformChanged(flags);
        }
    }
}

void Actor::addAndInitializeComponent(Component *component) {
    if (mAttachedComponent.contains(component)) {
        q_delete(component);
        return;
    }

    // component->initialize(component->getId());

    mAttachedComponent.add(component);

    gSceneObjectManager().registerObject(component);
    gSceneManager().notifyComponentCreated(component, isActive());
}

void Actor::destroyInternal(bool immediate) {
    if (immediate) {
        for (auto &it : mAttachedActorList) {
            it->destroyInternal(true);
        }

        mAttachedActorList.clear();

        while (!mAttachedComponent.empty()) {
            auto component = mAttachedComponent.top();
            component->setIsDestroyed();

            if (isInitialized()) {
                gSceneManager().notifyComponentDestroyed(component, immediate);
            }

            component->destroyInternal(true);
            mAttachedComponent.removeAt(mAttachedComponent.length() - 1);

            q_delete(component);
        }

        // mAttachedComponent.clear();
        gSceneObjectManager().unregisterObject(this);
    } else {
        gSceneObjectManager().queueForDestroy(this);
    }
}

