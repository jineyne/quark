#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderTarget.h"
#include "Renderer/CameraBase.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"
#include "Misc/Module.h"
#include "SceneManager.g.h"

QCLASS()
class DLL_EXPORT SceneManager : public TModule<SceneManager> {
    GENERATED_BODY()

private:
    enum class EComponentStateEventType {
        Created = 0, Activated = 1, Disabled = 2, Destroyed = 3
    };

    enum class EComponentStateListType {
        NoList = 0, ActiveList = 1, InactiveList = 2, UnStartedList = 3
    };

    struct FComponentStateChange {
    public:
        Component *component;

        EComponentStateEventType type;

        FComponentStateChange(Component *component, const EComponentStateEventType &type)
                :component(component), type(type) {
        }
    };

public:
    Scene *mActiveScene;
    RenderTarget *mMainRT;

    TMap<CameraBase *, CameraBase *> mCameraList;

    TArray<CameraBase*> mMainCameraList;

    TArray<Component*> mActiveComponentList;
    TArray<Component*> mInactiveComponentList;
    TArray<Component*> mUnStartedComponentList;

    TArray<TArray<Component*> *> mComponentsPerState = {
            &mActiveComponentList, &mInactiveComponentList, &mUnStartedComponentList
    };

    TArray<FComponentStateChange> mStateChanges;

    EComponentState mComponentState = EComponentState::Running;

public:
    SceneManager();

public:
    void update();

    void loadScene(Scene *scene);
    void unloadScene(Scene *scene);
    void clearScene(bool forceAll = false);

    void setComponentState(const EComponentState &state);

    void notifyCameraCreated(CameraBase *camera);
    void notifyCameraRemoved(CameraBase *camera);

    void setMainRenderTarget(RenderTarget *rt);

    void notifyMainCameraStateChanged(CameraBase *camera);

    void notifyComponentCreated(Component *component, bool parentActive);
    void notifyComponentDestroyed(Component *component, bool immediate = false);

    void notifyComponentActivated(Component *component);
    void notifyComponentDeactivated(Component *component);

    void createNewActor(Actor *actor);

    void addToState(Component *component, EComponentStateListType state);

    void removeFromState(Component *component);

    const auto &getActiveScene() const { return mActiveScene; }

protected:
    void clear();

    void processState();

    void onShutDown();

private:
    void onMainRenderTargetResized();
};

DLL_EXPORT SceneManager &gSceneManager();