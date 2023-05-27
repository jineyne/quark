#include "SceneObjectManager.h"

uint32_t SceneObjectManager::NextObjectId = 1;

void SceneObjectManager::registerObject(SceneObject *object) {
    if (object->isInitialized()) {
        LOG(LogScene, Warning, TEXT("Trying to register already initialized object: %ls"), *object->getName());
        return;
    }

    if (object->isDestroyed()) {
        LOG(LogScene, Warning, TEXT("Trying to register already destroyed object: %ls"), *object->getName());
        return;
    }

    auto id = NextObjectId++;
    mRegisteredObjectMap.add(id, object);

    object->initialize(id);
}

void SceneObjectManager::unregisterObject(SceneObject *object) {
    mRegisteredObjectMap.remove(object->getObjectId());
}

void SceneObjectManager::queueForDestroy(SceneObject *object) {
    if (object->isDestroyed()) {
        LOG(LogScene, Warning, TEXT("Trying to add destroy queue already destroyed object: %ls"), *object->getName());
        return;
    }

    mQueuedObject.push(object);
}

void SceneObjectManager::destroyQueuedObject() {
    while (!mQueuedObject.empty()) {
        auto object = mQueuedObject.front();
        mQueuedObject.pop();

        object->destroyInternal(true);
    }
}

SceneObject *SceneObjectManager::getObject(uint32_t id) {
    auto it = mRegisteredObjectMap.find(id);
    if (it == nullptr) {
        return nullptr;
    }

    return *it;
}

void SceneObjectManager::onShutDown() {
    for (auto &pair : mRegisteredObjectMap) {
        auto obj = pair.value;
        if (obj->isActive()) {
            obj->destroyInternal();
        }
    }

    destroyQueuedObject();
}

SceneObjectManager &gSceneObjectManager() {
    return SceneObjectManager::Instance();
}
