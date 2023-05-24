#pragma once

#include "CorePrerequisites.h"
#include "Scene/SceneObject.h"
#include "Misc/Module.h"
#include "SceneObjectManager.g.h"

QCLASS()
class DLL_EXPORT FSceneObjectManager : public TModule<FSceneObjectManager> {
    GENERATED_BODY()
public:
    using ObjectMap = TMap<uint32_t, FSceneObject *>;

private:
    static uint32_t NextObjectId;
    ObjectMap mRegisteredObjectMap;
    std::queue<FSceneObject *> mQueuedObject;

public:
    void registerObject(FSceneObject *object);
    void unregisterObject(FSceneObject *object);

    void queueForDestroy(FSceneObject *object);
    void destroyQueuedObject();

    FSceneObject *getObject(uint32_t id);

    uint32_t getObjectCount() const { return static_cast<uint32_t>(mRegisteredObjectMap.length()); }

protected:
    void onShutDown() override;
};

DLL_EXPORT FSceneObjectManager &gSceneObjectManager();