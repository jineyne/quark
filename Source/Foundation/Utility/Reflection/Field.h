#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/String.h"

#include "Reflection/MetaData.h"
#include "Reflection/Object.h"

enum EFieldFlags {
    FieldFlags_AccessMask   = 3,        // 0011
    FieldFlags_Public       = 0,        // 0000
    FieldFlags_Protected    = 1,        // 0001
    FieldFlags_Private      = 2,        // 0010

    FieldFlags_Pointer      = 1 << 2,   // 0100
};

class DLL_EXPORT Field : public Object {
private:
    MetaData mMetaData;
    uint64_t mFlags = 0;

public:
    Field(class Class *myClass, const String &name, uint64_t flags);

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

