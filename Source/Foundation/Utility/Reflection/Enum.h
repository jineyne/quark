#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Field.h"

class DLL_EXPORT QEnum : public QField {
protected:
    static TMap<FString, QEnum *> AllEnumByNameMap;

protected:
    TArray<std::pair<FString, int64_t>> mEntryList;

public:
    FString cppType = "";

public:
    QEnum(class QClass *myClass, const FString &name);

public:
    static bool LookupEnum(const FString &name, QEnum **found = nullptr);
    static int64_t LookupEnumName(const FString &name, QEnum **found = nullptr);
    static int64_t LookupEnumNameSlow(const FString &name, QEnum **found = nullptr);
    static int64_t ParseEnum(const FString &str);

public:
    virtual bool setEntries(TArray<std::pair<FString, int64_t>>& entry);

    int64_t getValueByName(const FString &name) const;
    int64_t getValueByIndex(int32_t index) const;
    const FString &getNameByValue(int64_t value) const;

    bool isValidName(const FString &name) const;
    bool isValidIndex(int32_t index) const;
    bool isValidEnumValue(int64_t value) const;

    void syncWithGlobal();
};