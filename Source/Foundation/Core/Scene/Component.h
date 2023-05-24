#pragma once

#include "CorePrerequisites.h"
#include "SceneObject.h"
#include "Component.g.h"

QCLASS()
class DLL_EXPORT FComponent : public FSceneObject {
    GENERATED_BODY()

private:
    FActor *mOwnerActor = nullptr;

public:
    FComponent() = default;
    virtual ~FComponent() = default;

public:
    virtual void onTransformChanged(const ETransformChangedFlags &flags) {};

    FActor *getOwner() const { return mOwnerActor; }

public:
    void attachTo(FActor *actor);
    void detachFrom(FActor *actor);

    void destroy(bool immediate = false) override;

    void setActive(bool active) override;

public:
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onStart() {}
    virtual void onStop() {}

    virtual void onActive() {}
    virtual void onDeactive() {}

    virtual void onPreUpdate() {}
    virtual void onUpdate() {}
    virtual void onPostUpdate() {}

    FTransform *getTransform() const;

protected:
    void destroyInternal(bool immediate) override;

private:
    friend class FActor;
};
