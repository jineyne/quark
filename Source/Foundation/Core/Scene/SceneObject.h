#pragma once

#include "CorePrerequisites.h"
#include "SceneTypes.h"
#include "SceneObject.g.h"

QCLASS()
class DLL_EXPORT FSceneObject : public QObject {
    GENERATED_BODY();

protected:
    QPROPERTY()
    uint32_t mObjectId = -1;

    QPROPERTY()
    uint32_t mState = 0;

    bool mInitialized = false;
    bool bDestroyed = false;

    QPROPERTY()
    bool mActiveSelf = true;

public:
    FSceneObject() = default;
    virtual ~FSceneObject();

public:
    bool operator==(const FSceneObject &rhs) const;
    bool operator!=(const FSceneObject &rhs) const;

public:
    void initialize(uint32_t id);

    virtual void destroy(bool immediate = false);

    auto getObjectId() const { return mObjectId; }

    void setState(uint32_t state);
    auto getState() const { return mState; }

    virtual void setActive(bool active);
    bool isActive() const { return mActiveSelf; }

    auto isInitialized() const { return mInitialized; }
    auto isDestroyed() const { return bDestroyed; }

    virtual void notifyTransformChanged(ETransformChangedFlags flags) const {}

protected:
    virtual void destroyInternal(bool immediate = false);

private:
    friend class FSceneObjectManager;
};
