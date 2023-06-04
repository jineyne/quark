#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AINode.h"

class DLL_EXPORT AIBlackboard {
private:
    enum class EEntryType {
        Unknown,
        Bool,
        Int,
        Float,
        Vector3,
        String,
        Object
    };

    struct Entry {
        EEntryType type;
        std::any data;
    };

    TMap<String, Entry> mData;

public:
    bool isSet(String key);
    EEntryType getType(String key);

    void clear();

    void setValueAsBool(String key, bool value);
    bool getValueAsBool(String key);

    void setValueAsInt(String key, int value);
    int getValueAsInt(String key);

    void setValueAsFloat(String key, float value);
    float getValueAsFloat(String key);

    void setValueAsVector3(String key, Vector3 value);
    Vector3 getValueAsVector3(String key);

    void setValueAsString(String key, String value);
    String getValueAsString(String key);

    void setValueAsObject(String key, Object *value);
    Object *getValueAsObject(String key);
};