#include "SceneManager.h"
#include "Scene/Actor.h"
#include "SceneObjectManager.h"

uint32_t encode(uint32_t idx, uint32_t type) {
    assert(idx <= (0x3fffffff));
    return (type << 30) | idx;
}

void decode(uint32_t id, uint32_t &idx, uint32_t &type) {
    idx = id & 0x3fffffff;
    type = id >> 30;
}

SceneManager::SceneManager() {
    mActiveScene = Scene::New();
}

void SceneManager::update() {
    processState();

    if (mActiveScene != nullptr) {
        for (auto component : mActiveComponentList) {
            component->onPreUpdate();
        }

        for (auto component : mActiveComponentList) {
            component->onUpdate();
        }

        for (auto component : mActiveComponentList) {
            component->onPostUpdate();
        }
    }

    gSceneObjectManager().destroyQueuedObject();
}

void SceneManager::loadScene(Scene *scene) {
    if (scene == nullptr) {
        return;
    }

    clear();

    mActiveScene = scene;
}

void SceneManager::unloadScene(Scene *scene) {
    if (scene == nullptr || mActiveScene == nullptr || scene != mActiveScene) {
        return;
    }

    mActiveScene->shutDown();
    mActiveScene = nullptr;
}

void SceneManager::clearScene(bool forceAll) {

}

void SceneManager::setComponentState(const EComponentState &state) {
    if (mComponentState == state) {
        return;
    }

    auto oldState = mComponentState;
    mComponentState = state;

    processState();

    if (state == EComponentState::Running || state == EComponentState::Paused) {
        if (oldState == EComponentState::Stopped) {
            for (auto &entry : mActiveComponentList) {
                if (entry->getOwner()->isActive()) {
                    entry->onDeactive();
                    entry->onActive();
                }
            }

            processState();

            for (auto &entry : mInactiveComponentList) {
                if (entry->isActive()) {
                    entry->onActive();

                    if (state == EComponentState::Running) {
                        mStateChanges.emplace(entry, EComponentStateEventType::Activated);
                    }
                }
            }

            processState();

            for (auto &entry : mUnStartedComponentList) {
                entry->onCreate();

                if (entry->getOwner()->isActive()) {
                    entry->onActive();
                    mStateChanges.emplace(entry, EComponentStateEventType::Activated);
                } else {
                    mStateChanges.emplace(entry, EComponentStateEventType::Disabled);
                }
            }

            processState();
        }
    }

    if (state == EComponentState::Paused || state == EComponentState::Stopped) {
        if (state == EComponentState::Stopped) {
            for (const auto &component : mActiveComponentList) {
                component->onDeactive();
            }
        }

        for (int32_t i = 0; i < static_cast<int32_t>(mActiveComponentList.length()); i++)
        {
            // Note: Purposely not a reference since the list changes in the add/remove methods below
            auto component = mActiveComponentList[i];

            /*const bool alwaysRun = component->hasFlag(ComponentFlag::AlwaysRun);
            if (alwaysRun)
                continue;*/

            removeFromState(component);
            addToState(component, EComponentStateListType::InactiveList);

            i--; // Keep the same index next iteration to process the component we just swapped
        }
    }
}

void SceneManager::notifyCameraCreated(CameraBase *camera) {
    mCameraList[camera] = camera;
}

void SceneManager::notifyCameraRemoved(CameraBase *camera) {
    mCameraList.remove(camera);

    auto it = std::find_if(mMainCameraList.begin(), mMainCameraList.end(),
                           [&](CameraBase *x) {
                               return x == camera;
                           });

    if (it != mMainCameraList.end()) {
        mMainCameraList.removeIt(it);
    }
}

void SceneManager::setMainRenderTarget(RenderTarget *rt) {
    if (mMainRT == rt) {
        return;
    }

    if (mMainRT != nullptr) {
        // mMainRT->onResized.unbind(&SceneManager::onMainRenderTargetResized, this);
    }

    if (rt != nullptr) {
        // rt->onResized.bind(&SceneManager::onMainRenderTargetResized, this);
    }

    mMainRT = rt;

    float aspect = 1.0f;
    if (rt != nullptr) {
        aspect = static_cast<float>(rt->getWidth()) / static_cast<float>(rt->getHeight());
    }

    for (auto &entry : mMainCameraList) {
        entry->getViewport()->setTarget(rt);
        entry->setAspectRatio(aspect);
    }
}

void SceneManager::notifyMainCameraStateChanged(CameraBase *camera) {
    auto iterFind = std::find_if(mMainCameraList.begin(), mMainCameraList.end(),
                                 [&](CameraBase *entry) {
                                     return entry == camera;
                                 });

    Viewport *viewport = camera->getViewport();
    if (camera->isMain()) {
        if (iterFind == mMainCameraList.end())
            mMainCameraList.add(mCameraList[camera]);

        viewport->setTarget(mMainRT);
    } else {
        if (iterFind != mMainCameraList.end())
            mMainCameraList.removeIt(iterFind);

        if (viewport->getTarget() == mMainRT)
            viewport->setTarget(nullptr);
    }
}

void SceneManager::notifyComponentCreated(Component *component, bool parentActive) {
    mStateChanges.emplace(component, EComponentStateEventType::Created);

    component->onCreate();

    if (mComponentState != EComponentState::Stopped) {
        component->onStart();

        if (parentActive) {
            component->onActive();
            component->setActive(true);
        }
    }
}

void SceneManager::notifyComponentDestroyed(Component *component, bool immediate) {
    if (!immediate) {
        mStateChanges.emplace(component, EComponentStateEventType::Destroyed);
    }

    const bool isEnabled = component->getOwner()->isActive() && mComponentState != EComponentState::Stopped;

    if (isEnabled) {
        component->onDeactive();
    }

    component->onDestroy();

    if (immediate) {
        uint32_t existingListType;
        uint32_t existingIdx;
        decode(component->getState(), existingIdx, existingListType);

        if (existingListType != 0) {
            removeFromState(component);
        }
    }
}

void SceneManager::notifyComponentActivated(Component *component) {
    mStateChanges.emplace(component, EComponentStateEventType::Activated);

    if (mComponentState != EComponentState::Stopped) {
        component->onActive();
    }
}

void SceneManager::notifyComponentDeactivated(Component *component) {
    mStateChanges.emplace(component, EComponentStateEventType::Disabled);

    if (mComponentState != EComponentState::Stopped) {
        component->onDeactive();
    }
}

void SceneManager::createNewActor(Actor *actor) {
    if (mActiveScene != nullptr) {
        actor->setScene(mActiveScene);
        actor->attachTo(mActiveScene->getRootActor());
    }
}

void SceneManager::addToState(Component *component, SceneManager::EComponentStateListType state) {
    if (state == EComponentStateListType::NoList) {
        return;
    }

    int _state = static_cast<int>(state);

    auto &list = *mComponentsPerState[_state - 1];
    const auto idx = static_cast<uint32_t>(list.length());
    list.add(component);

    component->setState(encode(idx, _state));
}

void SceneManager::removeFromState(Component *component) {
    uint32_t listType;
    uint32_t idx;
    decode(component->getState(), idx, listType);

    if (listType == 0) {
        return;
    }

    auto &list = *mComponentsPerState[listType - 1];

    uint32_t lastIdx;
    decode(list.top()->getState(), lastIdx, listType);

    assert(list[idx] == component);

    if (idx != lastIdx) {
        std::swap(list[idx], list[lastIdx]);
        list[idx]->setState(encode(idx, listType));
    }

    list.removeAt(list.length() - 1);
}

void SceneManager::clear() {
    if (mActiveScene != nullptr) {
        mActiveScene->shutDown();

        gSceneObjectManager().destroyQueuedObject();
        mActiveScene = nullptr;
    }
}

void SceneManager::processState() {
    const bool isStopped = mComponentState == EComponentState::Stopped;

    for (auto &entry : mStateChanges) {
        auto &component = entry.component;
        if (component == nullptr) {
            continue;
        }

        uint32_t existingListType;
        uint32_t existingIdx;
        decode(component->getState(), existingIdx, existingListType);

        const bool isActive = component->getOwner()->isActive();

        EComponentStateListType listType = EComponentStateListType::NoList;

        switch (entry.type) {
            case EComponentStateEventType::Created:
                if (!isStopped) {
                    listType = isActive ? EComponentStateListType::ActiveList : EComponentStateListType::InactiveList;
                } else {
                    listType = EComponentStateListType::UnStartedList;
                }
                break;

            case EComponentStateEventType::Activated:
            case EComponentStateEventType::Disabled:
                if (!isStopped) {
                    listType = isActive ? EComponentStateListType::ActiveList : EComponentStateListType::InactiveList;
                } else {
                    listType = (existingListType == static_cast<uint32_t>(EComponentStateListType::UnStartedList))
                               ? EComponentStateListType::UnStartedList
                               : EComponentStateListType::InactiveList;
                }
                break;

            case EComponentStateEventType::Destroyed:
                listType = EComponentStateListType::NoList;
                break;

            default:
                break;
        }

        if (static_cast<EComponentStateListType>(existingListType) == listType) {
            continue;
        }

        if (existingListType != 0) {
            removeFromState(component);
        }

        addToState(component, listType);
    }

    mStateChanges.clear();
}

void SceneManager::onShutDown() {
    clear();

    processState();
}

void SceneManager::onMainRenderTargetResized() {
    float aspect = static_cast<float>(mMainRT->getWidth()) / static_cast<float>(mMainRT->getHeight());

    for (auto &entry : mMainCameraList) {
        entry->setAspectRatio(aspect);
    }
}

SceneManager &gSceneManager() {
    return SceneManager::Instance();
}
