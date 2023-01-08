#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/Map.h"
#include "Container/String.h"

struct FMetaDataEntry;

using FMetaDataEntryMap = TMap<FString, FMetaDataEntry>;

struct DLL_EXPORT FMetaDataEntry {
    FString value;
    FMetaDataEntryMap child;
};


class DLL_EXPORT FMetaData {
public:
    FMetaDataEntryMap entries;

public:
    FMetaData() = default;

    ~FMetaData() = default;

public:
    bool contains(FString key);

    FString getValue(FString key);
    void setValue(FString key, FString value);

    bool tryGetEntry(FString key, FMetaDataEntry &out);
};


