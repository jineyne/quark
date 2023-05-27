#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Field.h"

class DLL_EXPORT Enum : public Field {
protected:
    static TMap<String, Enum *> AllEnumByNameMap;

protected:
    TMap<String, int64_t> mEntryList;

public:
    String cppType = "";

public:
    Enum(class Class *myClass, const String &name);

public:
    static bool LookupEnum(const String &name, Enum **found = nullptr);
    static int64_t LookupEnumName(const String &name, Enum **found = nullptr);
    static int64_t LookupEnumNameSlow(const String &name, Enum **found = nullptr);
    static int64_t ParseEnum(const String &str);

public:
    virtual bool setEntries(TMap<String, int64_t>& entry);

    int64_t getValueByName(const String &name) const;
    int64_t getValueByIndex(int32_t index) const;
    const String &getNameByValue(int64_t value) const;

    bool isValidName(const String &name) const;
    bool isValidIndex(int32_t index) const;
    bool isValidEnumValue(int64_t value) const;

    void syncWithGlobal();
};