#pragma once

#include "CorePrerequisites.h"
#include "Scene.g.h"

QCLASS()
class DLL_EXPORT FScene : public QObject {
    GENERATED_BODY();

private:
    FActor *mRootActor;
    uint32_t mNextObjectId = 1;

public:
    FScene();
    virtual ~FScene() = default;

public:
    static FScene *New();

public:
    void shutDown();

    template <typename T>
    T *spawnActor(const FString &name) {
        auto actor = q_new<T>(name);
        actor->initialize(mNextObjectId++);
        instantiate(actor);

        return actor;
    }

    const auto &getRootActor() const { return mRootActor; }

private:
    void instantiate(FActor *actor);

    friend class FSceneManager;
};
