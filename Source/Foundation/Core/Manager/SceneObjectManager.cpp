#include "SceneObjectManager.h"

uint32_t FSceneObjectManager::NextObjectId = 1;

void FSceneObjectManager::registerObject(FSceneObject *object) {
    if (object->isInitialized()) {
        LOG(FLogScene, Warning, TEXT("Trying to register already initialized object: %ls"), *object->getName());
        return;
    }

    if (object->isDestroyed()) {
        LOG(FLogScene, Warning, TEXT("Trying to register already destroyed object: %ls"), *object->getName());
        return;
    }

    auto id = NextObjectId++;
    mRegisteredObjectMap.add(id, object);

    object->initialize(id);
}

void FSceneObjectManager::unregisterObject(FSceneObject *object) {
    mRegisteredObjectMap.remove(object->getObjectId());
}

void FSceneObjectManager::queueForDestroy(FSceneObject *object) {
    if (object->isDestroyed()) {
        LOG(FLogScene, Warning, TEXT("Trying to add destroy queue already destroyed object: %ls"), *object->getName());
        return;
    }

    mQueuedObject.push(object);
}

void FSceneObjectManager::destroyQueuedObject() {
    while (!mQueuedObject.empty()) {
        auto object = mQueuedObject.front();
        mQueuedObject.pop();

        object->destroyInternal(true);
    }
}

FSceneObject *FSceneObjectManager::getObject(uint32_t id) {
    auto it = mRegisteredObjectMap.find(id);
    if (it == nullptr) {
        return nullptr;
    }

    return *it;
}

void FSceneObjectManager::onShutDown() {
    for (auto &pair : mRegisteredObjectMap) {
        auto obj = pair.value;
        if (obj->isActive()) {
            obj->destroyInternal();
        }
    }

    destroyQueuedObject();
}

FSceneObjectManager &gSceneObjectManager() {
    return FSceneObjectManager::Instance();
}
