#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/Map.h"
#include "Container/String.h"

struct MetaDataEntry;

using MetaDataEntryMap = TMap<String, MetaDataEntry>;

struct DLL_EXPORT MetaDataEntry {
    String value;
    MetaDataEntryMap child;
};


class DLL_EXPORT MetaData {
public:
    MetaDataEntryMap entries;

public:
    MetaData() = default;

    ~MetaData() = default;

public:
    bool contains(String key);

    String getValue(String key);
    void setValue(String key, String value);

    bool tryGetEntry(String key, MetaDataEntry &out);
};


