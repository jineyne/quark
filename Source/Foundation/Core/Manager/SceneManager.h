#pragma once

#include "CorePrerequisites.h"
#include "RenderAPI/RenderTarget.h"
#include "Renderer/CameraBase.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"
#include "Misc/Module.h"
#include "SceneManager.g.h"

QCLASS()
class DLL_EXPORT FSceneManager : public TModule<FSceneManager> {
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
        FComponent *component;

        EComponentStateEventType type;

        FComponentStateChange(FComponent *component, const EComponentStateEventType &type)
                :component(component), type(type) {
        }
    };

public:
    FScene *mActiveScene;
    FRenderTarget *mMainRT;

    TMap<FCameraBase *, FCameraBase *> mCameraList;

    TArray<FCameraBase*> mMainCameraList;

    TArray<FComponent*> mActiveComponentList;
    TArray<FComponent*> mInactiveComponentList;
    TArray<FComponent*> mUnStartedComponentList;

    TArray<TArray<FComponent*> *> mComponentsPerState = {
            &mActiveComponentList, &mInactiveComponentList, &mUnStartedComponentList
    };

    TArray<FComponentStateChange> mStateChanges;

    EComponentState mComponentState = EComponentState::Running;

public:
    FSceneManager();

public:
    void update();

    void loadScene(FScene *scene);
    void unloadScene(FScene *scene);
    void clearScene(bool forceAll = false);

    void setComponentState(const EComponentState &state);

    void notifyCameraCreated(FCameraBase *camera);
    void notifyCameraRemoved(FCameraBase *camera);

    void setMainRenderTarget(FRenderTarget *rt);

    void notifyMainCameraStateChanged(FCameraBase *camera);

    void notifyComponentCreated(FComponent *component, bool parentActive);
    void notifyComponentDestroyed(FComponent *component, bool immediate = false);

    void notifyComponentActivated(FComponent *component);
    void notifyComponentDeactivated(FComponent *component);

    void createNewActor(FActor *actor);

    void addToState(FComponent *component, EComponentStateListType state);

    void removeFromState(FComponent *component);

    const auto &getActiveScene() const { return mActiveScene; }

protected:
    void clear();

    void processState();

    void onShutDown();

private:
    void onMainRenderTargetResized();
};

DLL_EXPORT FSceneManager &gSceneManager();