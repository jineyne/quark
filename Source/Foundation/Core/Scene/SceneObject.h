#pragma once

#include "CorePrerequisites.h"
#include "SceneTypes.h"
#include "SceneObject.g.h"

QCLASS()
class DLL_EXPORT SceneObject : public Object {
    GENERATED_BODY();

protected:
    QPROPERTY()
    uint32_t mObjectId = -1;

    QPROPERTY()
    String mTags;

    QPROPERTY()
    uint32_t mState = 0;

    bool mInitialized = false;
    bool bDestroyed = false;

    QPROPERTY()
    bool mActiveSelf = true;

public:
    SceneObject() = default;
    virtual ~SceneObject();

public:
    bool operator==(const SceneObject &rhs) const;
    bool operator!=(const SceneObject &rhs) const;

public:
    void initialize(uint32_t id);

    virtual void destroy(bool immediate = false);

    auto getObjectId() const { return mObjectId; }

    const String &getTags() const;
    void setTags(const String &tags);

    void setState(uint32_t state);
    auto getState() const { return mState; }

    virtual void setActive(bool active);
    bool isActive() const { return mActiveSelf; }

    auto isInitialized() const { return mInitialized; }

    void setIsDestroyed();
    auto isDestroyed() const { return bDestroyed; }

    virtual void notifyTransformChanged(ETransformChangedFlags flags) const {}

protected:
    virtual void destroyInternal(bool immediate = false);

private:
    friend class SceneObjectManager;
};
