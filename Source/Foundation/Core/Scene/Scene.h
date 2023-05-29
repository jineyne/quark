#pragma once

#include "CorePrerequisites.h"
#include "Scene.g.h"

QCLASS()
class DLL_EXPORT Scene : public Object {
    GENERATED_BODY();

private:
    Actor *mRootActor;
    uint32_t mNextObjectId = 1;

public:
    Scene();
    virtual ~Scene() = default;

public:
    static Scene *New();

public:
    void shutDown();

    const auto &getRootActor() const { return mRootActor; }

private:
    void instantiate(Actor *actor);

    friend class SceneManager;
};
