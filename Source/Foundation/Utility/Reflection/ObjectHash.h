#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/Map.h"
#include "Object.h"

class DLL_EXPORT FObjectHash {
private:
    static size_t NextId;
    TMap<FString, QObject *> mObjectMap;

public:
    void add(QObject *object);
    void remove(QObject *object);

    QObject *find(FString name, QObject *outer);
};

DLL_EXPORT FObjectHash &gObjectHash();

