#include "MetaData.h"

#include "Logging/LogDefines.h"

#include "Object.h"
#include "Reflection.h"

bool FMetaData::contains(FString key) {
    if (key.empty()) {
        return false;
    }


    auto paths = key.split(".");
    if (paths.empty()) {
        return entries.find(key) != nullptr;
    }

    if (entries.find(paths[0]) == nullptr) {
        return false;
    }

    FMetaDataEntry *entry = &entries[paths[0]];
    for (auto i = 1; i < paths.length(); i++) {
        if (entry->child.find(paths[i]) == nullptr) {
            return false;
        }
        entry = &(entry->child[paths[i]]);
    }

    return true;
}

FString FMetaData::getValue(FString key) {
    if (key.empty()) {
        return "";
    }

    auto paths = key.split(TEXT("."));
    if (paths.empty()) {
        return entries[key].value;
    }

    if (!contains(paths[0])) {
        entries.add(paths[0], FMetaDataEntry());
    }

    FMetaDataEntry *entry = &entries[paths[0]];
    for (auto i = 1; i < paths.length(); i++) {
        if (entry->child.find(paths[i]) == nullptr) {
            entry->child.add(paths[i], FMetaDataEntry());
        }
        entry = &(entry->child[paths[i]]);
    }

    return entry->value;
}

void FMetaData::setValue(FString key, FString value) {
    if (key.empty()) {
        LOG(LogReflection, Error, TEXT("Key is empty"));
    }

    auto paths = key.split(TEXT("."));
    if (paths.empty()) {
        entries[key].value = value;
    } else {
        if (!contains(paths[0])) {
            entries.add(paths[0], FMetaDataEntry());
        }

        FMetaDataEntry *entry = &entries[paths[0]];
        for (auto i = 1; i < paths.length(); i++) {
            if (entry->child.find(paths[i]) == nullptr) {
                entry->child.add(paths[i], FMetaDataEntry{});
            }
            entry = &(entry->child[paths[i]]);
        }

        entry->value = value;
    }
}

bool FMetaData::tryGetEntry(FString key, FMetaDataEntry &out) {
    if (!contains(key)) {
        return false;
    }

    auto paths = key.split(TEXT("."));
    if (paths.empty()) {
        out = entries[key];
        return true;
    }

    if (!contains(paths[0])) {
        return false;
    }

    FMetaDataEntry *entry = &entries[paths[0]];
    for (auto i = 1; i < paths.length(); i++) {
        if (entry->child.find(paths[i]) == nullptr) {
            return false;
        }
        entry = &(entry->child[paths[i]]);
    }

    out = *entry;
    return true;
}