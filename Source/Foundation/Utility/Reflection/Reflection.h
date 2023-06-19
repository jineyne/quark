#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Logging/LogDefines.h"

#include "Reflection/Class.h"
#include "Reflection/Enum.h"
#include "Reflection/Field.h"
#include "Reflection/Object.h"
#include "Reflection/Property.h"
#include "Reflection/Struct.h"

DECLARE_LOG_CATEGORY_EXTERN(LogReflection, Debug);

class DLL_EXPORT Reflection {
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

    struct MetaDataPairDesc {
        const TCHAR *name;
        const TCHAR *value;
    };

    struct EnumEntry {
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

    struct PropertyDescBase {
        PROPERTY_BASE;
    };

    struct PropertyDescBaseWithOffset {
        PROPERTY_BASE_WITH_OFFSET;
    };

    struct GenericPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        TArray<MetaDataPairDesc> metas;
    };

    struct ArrayPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        Property *property = nullptr;

        TArray<MetaDataPairDesc> metas;
    };

    struct MapPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        Property *keyProperty = nullptr;
        Property *valueProperty = nullptr;

        TFunction<void(uint8_t *, uint8_t *, uint8_t *)> fnAdd;

        TArray<MetaDataPairDesc> metas;
    };

    struct ObjectPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        TArray<MetaDataPairDesc> metas;
    };

    struct StructPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        Struct *(*fnStructConstructor)();

        TArray<MetaDataPairDesc> metas;
    };

    struct ClassPropertyDesc {
        PROPERTY_BASE_WITH_OFFSET;

        Class *(*fnClassConstructor)();

        TArray<MetaDataPairDesc> metas;
    };

    struct EnumDesc {
        const String name;
        const String cppType;
        const TArray<EnumEntry> entries;

        TArray<MetaDataPairDesc> metas;
    };

    struct StructDesc {
        const String name;
        Struct *(*fnNoRegister)();
        EStructFlags classFlags;
        size_t size;
        const TArray<PropertyDescBase const*> properties;

        TArray<MetaDataPairDesc> metas;
    };

    struct ClassDesc {
        const String name;
        Class *(*fnNoRegister)();
        EClassFlags classFlags;
        const TArray<PropertyDescBase const*> properties;
        // TODO: FUNCTION DESC

        TArray<MetaDataPairDesc> metas;
    };

public:
    static void Initialize();

    static Object *InitObject(Object *target, Object *parent, Class *clazz, String name, EObjectFlags flags);

    static void RegisterClass(Class *(*fnRegister)(), Class *(*fnStaticClass)(), const String &name);
    static void RegisterStruct(Struct *(*fnRegister)(), Struct *(*fnStaticStruct)(), const String &name);
    static void GetPrivateStaticClass(Class *&instance, void (*fnInitNativeClass)(), Class::ClassConstructorType fnClassConstructor, size_t size, const String &name, Class *(*fnSuperStaticClass)());

    static void CreateProperty(Struct *target, const PropertyDescBase *desc);
    static void CreateEnum(Enum *&target, const EnumDesc &desc);
    static void CreateStruct(Struct *&target, const StructDesc &desc);
    static void CreateClass(Class *&target, const ClassDesc &desc);

    static TMap<Class *, const ClassDesc *> &GetClassDescMap();
    static TMap<Struct *, const StructDesc *> &GetStructDescMap();

private:
    static TArray<Class *(*)()> &GetClassRegisterList();
    static TArray<Struct *(*)()> &GetStructRegisterList();

};

template <typename T>
T *newObject(Object *outer = nullptr, Class *clazz = T::StaticClass(), String name = "", EObjectFlags flags = ObjectFlag_None) {
    Object *instance = (Object *) (q_new<T>());
    return (T *) Reflection::InitObject(instance, outer, clazz, name, flags);
}

