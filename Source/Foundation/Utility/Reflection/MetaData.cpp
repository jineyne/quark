#include "MetaData.h"

#include "Logging/LogDefines.h"

#include "Object.h"
#include "Reflection.h"

bool MetaData::contains(String key) {
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

    MetaDataEntry *entry = &entries[paths[0]];
    for (auto i = 1; i < paths.length(); i++) {
        if (entry->child.find(paths[i]) == nullptr) {
            return false;
        }
        entry = &(entry->child[paths[i]]);
    }

    return true;
}

String MetaData::getValue(String key) {
    if (key.empty()) {
        return "";
    }

    auto paths = key.split(TEXT("."));
    if (paths.empty()) {
        return entries[key].value;
    }

    if (!contains(paths[0])) {
        entries.add(paths[0], MetaDataEntry());
    }

    MetaDataEntry *entry = &entries[paths[0]];
    for (auto i = 1; i < paths.length(); i++) {
        if (entry->child.find(paths[i]) == nullptr) {
            entry->child.add(paths[i], MetaDataEntry());
        }
        entry = &(entry->child[paths[i]]);
    }

    return entry->value;
}

void MetaData::setValue(String key, String value) {
    if (key.empty()) {
        LOG(LogReflection, Error, TEXT("Key is empty"));
    }

    auto paths = key.split(TEXT("."));
    if (paths.empty()) {
        entries[key].value = value;
    } else {
        if (!contains(paths[0])) {
            entries.add(paths[0], MetaDataEntry());
        }

        MetaDataEntry *entry = &entries[paths[0]];
        for (auto i = 1; i < paths.length(); i++) {
            if (entry->child.find(paths[i]) == nullptr) {
                entry->child.add(paths[i], MetaDataEntry{});
            }
            entry = &(entry->child[paths[i]]);
        }

        entry->value = value;
    }
}

bool MetaData::tryGetEntry(String key, MetaDataEntry &out) {
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

    MetaDataEntry *entry = &entries[paths[0]];
    for (auto i = 1; i < paths.length(); i++) {
        if (entry->child.find(paths[i]) == nullptr) {
            return false;
        }
        entry = &(entry->child[paths[i]]);
    }

    out = *entry;
    return true;
}