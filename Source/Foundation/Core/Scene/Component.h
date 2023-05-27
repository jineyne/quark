#pragma once

#include "CorePrerequisites.h"
#include "SceneObject.h"
#include "Component.g.h"

QCLASS()
class DLL_EXPORT Component : public SceneObject {
    GENERATED_BODY()

private:
    Actor *mOwnerActor = nullptr;

public:
    Component() = default;
    virtual ~Component() = default;

public:
    virtual void onTransformChanged(const ETransformChangedFlags &flags) {};

    Actor *getOwner() const { return mOwnerActor; }

public:
    void attachTo(Actor *actor);
    void detachFrom(Actor *actor);

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

    Transform *getTransform() const;

protected:
    void destroyInternal(bool immediate) override;
    void setIsDestroyed();

private:
    friend class Actor;
};
