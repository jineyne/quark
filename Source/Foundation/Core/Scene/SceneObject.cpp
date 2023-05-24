#include "SceneObject.h"
#include "Manager/SceneObjectManager.h"

FSceneObject::~FSceneObject() {
    if (isInitialized()) {
        gSceneObjectManager().unregisterObject(this);
    }
}

bool FSceneObject::operator==(const FSceneObject &rhs) const {
    return mObjectId == rhs.mObjectId;
}

bool FSceneObject::operator!=(const FSceneObject &rhs) const {
    return !(rhs == *this);
}

void FSceneObject::initialize(uint32_t id) {
    mInitialized = false;
    mObjectId = id;
}

void FSceneObject::destroy(bool immediate) {
    destroyInternal(immediate);
}

void FSceneObject::setState(uint32_t state) {
    mState = state;
}

void FSceneObject::setActive(bool active) {
    if (mActiveSelf != active) {
        mActiveSelf = active;
    }
}

void FSceneObject::destroyInternal(bool immediate) {
    if (immediate) {
        bDestroyed = true;

        gSceneObjectManager().unregisterObject(this);
    } else {
        gSceneObjectManager().queueForDestroy(this);
    }
}