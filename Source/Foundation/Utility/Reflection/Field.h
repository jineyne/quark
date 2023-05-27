#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/String.h"

#include "Reflection/MetaData.h"
#include "Reflection/Object.h"

enum EFieldFlags {
    FieldFlags_Public,
    FieldFlags_Protected,
    FieldFlags_Private,
};

class DLL_EXPORT Field : public Object {
private:
    MetaData mMetaData;
    uint64_t mFlags = 0;

public:
    Field(class Class *myClass, const String &name);

public:
    String getMetaData(const String &key);
    void setMetaData(const String &key, const String &value);
    bool hasMetaData(const String &key);
    bool tryMetaDataEntry(const String &key, MetaDataEntry &out);

    void setFlags(uint64_t flags) { flags = mFlags; }
    uint64_t getFlags() const { return mFlags; }

    // REFLECTION
public:
    DECLARE_CLASS(Field, Object, );
    static void StaticRegisterNativeField() {
    }
};

