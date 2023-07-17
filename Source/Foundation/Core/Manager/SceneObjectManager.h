#pragma once

#include "CorePrerequisites.h"
#include "Scene/SceneObject.h"
#include "Misc/Module.h"
#include "SceneObjectManager.g.h"

QCLASS()
class DLL_EXPORT SceneObjectManager : public TModule<SceneObjectManager> {
    GENERATED_BODY()
public:
    using ObjectMap = TMap<uint32_t, SceneObject *>;

private:
    uint32_t mNextObjectId = 1;
    ObjectMap mRegisteredObjectMap;
    TMap<uint64_t, SceneObject *> mQueuedObject;

public:
    void registerObject(SceneObject *object);
    void unregisterObject(SceneObject *object);

    void queueForDestroy(SceneObject *object);
    void destroyQueuedObject();

    SceneObject *getObject(uint32_t id);

    uint32_t getObjectCount() const { return static_cast<uint32_t>(mRegisteredObjectMap.length()); }

protected:
    void onShutDown() override;
};

DLL_EXPORT SceneObjectManager &gSceneObjectManager();