#include "SceneObject.h"
#include "Manager/SceneObjectManager.h"

SceneObject::~SceneObject() {
    if (isInitialized()) {
        gSceneObjectManager().unregisterObject(this);
    }
}

bool SceneObject::operator==(const SceneObject &rhs) const {
    return mObjectId == rhs.mObjectId;
}

bool SceneObject::operator!=(const SceneObject &rhs) const {
    return !(rhs == *this);
}

void SceneObject::initialize(uint32_t id) {
    mInitialized = true;
    mObjectId = id;
}

void SceneObject::destroy(bool immediate) {
    destroyInternal(immediate);
}

const String &SceneObject::getTags() const {
    return mTags;
}

void SceneObject::setTags(const String &tags) {
    mTags = tags;
}

void SceneObject::setState(uint32_t state) {
    mState = state;
}

void SceneObject::setActive(bool active) {
    if (mActiveSelf != active) {
        mActiveSelf = active;
    }
}

void SceneObject::setIsDestroyed() {
    bDestroyed = true;
}

void SceneObject::destroyInternal(bool immediate) {
    if (immediate) {
        bDestroyed = true;

        gSceneObjectManager().unregisterObject(this);
    } else {
        gSceneObjectManager().queueForDestroy(this);
    }
}