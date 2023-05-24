#include "Actor.h"
#include "Manager/SceneObjectManager.h"
#include "Manager/SceneManager.h"

FActor::FActor() {
    mTransform = FTransform::New(this);
}

FActor *FActor::New(const FString &actorName) {
    FActor *actor = newObject<FActor>(nullptr, FActor::StaticClass(), actorName);

    gSceneObjectManager().registerObject(actor);
    gSceneManager().createNewActor(actor);

    return actor;
}

void FActor::destroy(bool immediate) {
    if (mParentActor != nullptr) {
        if (!mParentActor->isDestroyed()) {
            detachFrom(mParentActor);
        }

        mParentActor = nullptr;
    }

    FSceneObject::destroy(immediate);
}

void FActor::attachTo(FActor *actor) {
    if (mParentActor != nullptr) {
        detachFrom(mParentActor);
    }

    mParentActor = actor;
    if (actor->mAttachedActorList.contains(this)) {
        return;
    }

    actor->mAttachedActorList.add(this);
}

void FActor::detachFrom(FActor *actor) {
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

void FActor::move(const FVector3 &value) {
    auto pos = mTransform->getPosition();
    mTransform->setPosition(pos + value);
}

void FActor::removeComponent(FComponent *component) {
    mAttachedComponent.remove(component);
}

void FActor::destroyComponent(FComponent *component, bool immediate) {
    if (component == nullptr) {
        LOG(FLogScene, Warning, TEXT("Trying to remove a null component"));
    }

    if (mAttachedComponent.contains(component)) {
        if (isInitialized()) {
            gSceneManager().notifyComponentDestroyed(component, immediate);
        }

        component->destroyInternal(immediate);
        mAttachedComponent.remove(component);
    }
}

void FActor::setActive(bool active) {
    if (mActiveSelf != active) {
        for (auto &child : mAttachedActorList) {
            child->setActive(active);
        }

        for (auto &component : mAttachedComponent) {
            component->setActive(active);
        }
    }

    FSceneObject::setActive(active);
}

void FActor::setScene(FScene *targetScene) {
    mTargetScene = targetScene;
}

void FActor::notifyTransformChanged(ETransformChangedFlags flags) const {
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

void FActor::addAndInitializeComponent(FComponent *component) {
    if (mAttachedComponent.contains(component)) {
        return;
    }

    mAttachedComponent.add(component);

    gSceneObjectManager().registerObject(component);
    gSceneManager().notifyComponentCreated(component, isActive());
}

void FActor::destroyInternal(bool immediate) {
    if (immediate) {
        for (auto &it : mAttachedActorList) {
            it->destroyInternal(true);
        }

        mAttachedActorList.clear();

        for (auto &it : mAttachedComponent) {
            it->destroyInternal(true);
        }

        mAttachedComponent.clear();
    }

    FSceneObject::destroyInternal(immediate);
}

