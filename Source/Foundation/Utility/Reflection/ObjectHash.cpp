#include "ObjectHash.h"

void FObjectHash::add(QObject *object) {
    mObjectMap[object->getName()] = object;
}

void FObjectHash::remove(QObject *object) {
    QObject **it = mObjectMap.find(object->getName());
    if (it == nullptr) {
        return;
    }

    mObjectMap.remove((*it)->getName());
}

QObject *FObjectHash::find(FString name, QObject *outer) {
    auto it = mObjectMap.find(name);
    if (it == nullptr) {
        return nullptr;
    }

    return *it;
}

FObjectHash &gObjectHash() {
    static FObjectHash instance;
    return instance;
}