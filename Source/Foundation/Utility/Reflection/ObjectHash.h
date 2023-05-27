#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/Map.h"
#include "Object.h"

class DLL_EXPORT ObjectHash {
private:
    static size_t NextId;
    TMap<String, Object *> mObjectMap;

public:
    void add(Object *object);
    void remove(Object *object);

    Object *find(String name, Object *outer);
};

DLL_EXPORT ObjectHash &gObjectHash();

