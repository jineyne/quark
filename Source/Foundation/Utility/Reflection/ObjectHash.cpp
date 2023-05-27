#include "ObjectHash.h"

size_t ObjectHash::NextId = 1;

void ObjectHash::add(Object *object) {
    object->setId(NextId++);
    mObjectMap[object->getName()] = object;
}

void ObjectHash::remove(Object *object) {
    Object **it = mObjectMap.find(object->getName());
    if (it == nullptr) {
        return;
    }

    mObjectMap.remove((*it)->getName());
}

Object *ObjectHash::find(String name, Object *outer) {
    auto it = mObjectMap.find(name);
    if (it == nullptr) {
        return nullptr;
    }

    return *it;
}

ObjectHash &gObjectHash() {
    static ObjectHash instance;
    return instance;
}