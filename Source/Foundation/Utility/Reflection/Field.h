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

class DLL_EXPORT QField : public QObject {
private:
    FMetaData mMetaData;
    uint64_t mFlags;

public:
    QField(class QClass *myClass, const FString &name);

public:
    FString getMetaData(const FString &key);
    void setMetaData(const FString &key, const FString &value);
    bool hasMetaData(const FString &key);
    bool tryMetaDataEntry(const FString &key, FMetaDataEntry &out);

    void setFlags(uint64_t flags) { flags = mFlags; }
    uint64_t getFlags() const { return mFlags; }

    // REFLECTION
public:
    DECLARE_CLASS(QField, QObject, );
    static void StaticRegisterNativeQField() {
    }
};

