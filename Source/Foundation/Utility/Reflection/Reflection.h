#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Logging/LogDefines.h"

#include "Reflection/Class.h"
#include "Reflection/Enum.h"
#include "Reflection/Field.h"
#include "Reflection/Property.h"
#include "Reflection/Struct.h"

DECLARE_LOG_CATEGORY_EXTERN(LogReflection, Debug);

class DLL_EXPORT QReflection {
public:
    enum class EPropertyGenFlags {
        None = 0x00,
        Byte,
        Int8,
        Int32,
        Int64,
        UInt8,
        UInt32,
        UInt64,
        Float,
        Double,
        Bool,
        String,
        Class,
        Object, // -> pointer object
        Struct,
        NativeArray, // -> mValue[]
        Array,
        Map,
        Set,
    };

    struct FMetaDataPairDesc {
        const TCHAR *name;
        const TCHAR *value;
    };

    struct FEnumEntry {
        const TCHAR *name;
        int64_t value;
    };

#define PROPERTY_BASE \
    const TCHAR *name; \
    EPropertyFlags propertyFlags; \
    EPropertyGenFlags flags;      \
    size_t size;                  \
    int32_t arraySize

#define PROPERTY_BASE_WITH_OFFSET \
    PROPERTY_BASE; \
    size_t offset

    struct FPropertyDescBase {
        PROPERTY_BASE;
    };

    struct FPropertyDescBaseWithOffset {
        PROPERTY_BASE_WITH_OFFSET;
    };

    struct FGenericPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        TArray<FMetaDataPairDesc> metas;
    };

    struct FArrayPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        QProperty *property = nullptr;

        TArray<FMetaDataPairDesc> metas;
    };

    struct FObjectPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        TArray<FMetaDataPairDesc> metas;
    };

    struct FStructPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        QStruct *(*fnStructConstructor)();

        TArray<FMetaDataPairDesc> metas;
    };

    struct FClassPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        QClass *(*fnClassConstructor)();

        TArray<FMetaDataPairDesc> metas;
    };

    struct FEnumDesc {
        const FString name;
        const FString cppType;
        const TArray<FEnumEntry> entries;

        TArray<FMetaDataPairDesc> metas;
    };

    struct FStructDesc {
        const FString name;
        QStruct *(*fnNoRegister)();
        EStructFlags classFlags;
        size_t size;
        const TArray<FPropertyDescBase const*> properties;

        TArray<FMetaDataPairDesc> metas;
    };

    struct FClassDesc {
        const FString name;
        QClass *(*fnNoRegister)();
        EClassFlags classFlags;
        const TArray<FPropertyDescBase const*> properties;
        // TODO: FUNCTION DESC

        TArray<FMetaDataPairDesc> metas;
    };

public:
    static void Initialize();

    static QObject *InitObject(QObject *target, QObject *parent, QClass *clazz, FString name, EObjectFlags flags);

    static void RegisterClass(QClass *(*fnRegister)(), QClass *(*fnStaticClass)(), const FString &name);
    static void RegisterStruct(QStruct *(*fnRegister)(), QStruct *(*fnStaticStruct)(), const FString &name);
    static void GetPrivateStaticClass(QClass *&instance, void (*fnInitNativeClass)(), QClass::ClassConstructorType fnClassConstructor, size_t size, const FString &name, QClass *(*fnSuperStaticClass)());

    static void CreateProperty(QStruct *target, const FPropertyDescBase *desc);
    static void CreateEnum(QEnum *&target, const FEnumDesc &desc);
    static void CreateStruct(QStruct *&target, const FStructDesc &desc);
    static void CreateClass(QClass *&target, const FClassDesc &desc);

    static TMap<QClass *, const FClassDesc *> &GetClassDescMap();
    static TMap<QStruct *, const FStructDesc *> &GetStructDescMap();

private:
    static TArray<QClass *(*)()> &GetClassRegisterList();
    static TArray<QStruct *(*)()> &GetStructRegisterList();

};

template <typename T>
T *newObject(QObject *outer = nullptr, QClass *clazz = T::StaticClass(), FString name = "", EObjectFlags flags = ObjectFlag_None) {
    QObject *instance = (QObject *) (q_new<T>());
    return (T *) QReflection::InitObject(instance, outer, clazz, name, flags);
}

