#include "Reflection.h"

#include "Container/Array.h"
#include "Container/String.h"

#include "Exception/Exception.h"

#include "Reflection/Class.h"
#include "Reflection/ObjectHash.h"

DEFINE_LOG_CATEGORY(LogReflection);

String makeUniqueObjectName(Object *parent, Class *clazz, const String &name) {
    assert(clazz);


    String baseName = name.empty() ? clazz->getName() : name;
    String testName;
    Object *found = nullptr;
    // TODO: check name

    do {
        Object *object;
        do {
            static std::atomic<int32_t> uniqueIdx = 0;
            testName = String::Printf(TEXT("%ls %d"), *baseName, ++uniqueIdx);
            // testName = fmt::format("{0} {1}", baseName, ++uniqueIdx);
            object = gObjectHash().find(testName, parent);
        } while (object);
    } while (testName == baseName);

    return testName;
}

void Reflection::Initialize() {
    auto structRegisterList = std::move(GetStructRegisterList());
    if (!structRegisterList.empty()) {
        for (auto &fnRegister : structRegisterList) {
            Struct *target = fnRegister();
        }
    }

    auto classRegisterList = std::move(GetClassRegisterList());
    if (!classRegisterList.empty()) {
        for (auto &fnRegister : classRegisterList) {
            Class *target = fnRegister();
            gObjectHash().add(target);
        }
    }
}

Object *Reflection::InitObject(Object *target, Object *parent, Class *clazz, String name, EObjectFlags flags) {
#if REFLECTION_ENABLE

    assert(target != nullptr);
    // assert(parent != nullptr);

    if (name.empty()) {
        name = makeUniqueObjectName(parent, (Class *) clazz, name);
    }

    Object *temp = gObjectHash().find(name, parent);
    if (temp) {
        LOG(LogReflection, Warning, TEXT("%s is already in %s"), *name, *temp->getName());
        name = makeUniqueObjectName(parent, clazz, name);
    }

    target->mName = name;

    target->setClass((Class *) clazz);

    gObjectHash().add(target);

#endif

    return target;
}

void Reflection::RegisterClass(Class *(*fnRegister)(), Class *(*fnStaticClass)(), const String &name) {
    GetClassRegisterList().add(fnRegister);
}

void Reflection::RegisterStruct(Struct *(*fnRegister)(), Struct *(*fnStaticStruct)(), const String &name) {
    GetStructRegisterList().add(fnRegister);
}

void Reflection::GetPrivateStaticClass(Class *&instance, void (*fnInitNativeClass)(), Class::ClassConstructorType fnClassConstructor, size_t size, const String &name, Class *(*fnSuperStaticClass)()) {
    if (gObjectHash().find(name, nullptr) != nullptr) {
        LOG(LogReflection, Fatal, TEXT("%s is duplicated class"), *name)
    }

    instance = q_new<Class>(size, fnClassConstructor);
    assert(instance);

    instance->setClass(instance);
    instance->mName = name;

    auto super = fnSuperStaticClass();
    if (super && super != instance) {
        instance->setSuperStruct(super);
        instance->parent = super;
    }

    instance->setFlags(0);
    fnInitNativeClass();
}

void Reflection::CreateProperty(Struct *target, const PropertyDescBase* desc) {
    auto offsetDesc = reinterpret_cast<const PropertyDescBaseWithOffset *>(desc);
    Property *instance = nullptr;
    TArray<MetaDataPairDesc> metas;

    switch (desc->flags) {
        case EPropertyGenFlags::Array: {
            auto arrayDesc = reinterpret_cast<const ArrayPropertyDesc *>(desc);
            auto property = q_new<ArrayProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            property->setTemplateType(arrayDesc->property);
            property->setClass(ArrayProperty::StaticClass());

            instance = property;
            metas = arrayDesc->metas;
        } break;

        case EPropertyGenFlags::Map: {
            auto mapDesc = reinterpret_cast<const MapPropertyDesc *>(desc);
            auto property = q_new<MapProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            property->setKeyType(mapDesc->keyProperty);
            property->setValueType(mapDesc->valueProperty);
            property->setAddFunction(mapDesc->fnAdd);
            property->setClass(ArrayProperty::StaticClass());

            instance = property;
            metas = mapDesc->metas;
        } break;

        case EPropertyGenFlags::Set:
            assert(false); // set support!
            break;

        case EPropertyGenFlags::Object:
            instance = q_new<ObjectProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(ObjectProperty::StaticClass());
            metas = reinterpret_cast<const ObjectPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Struct:
            instance = q_new<StructProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(StructProperty::StaticClass());
            metas = reinterpret_cast<const StructPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Class:
            instance = q_new<ClassProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(ClassProperty::StaticClass());
            metas = reinterpret_cast<const ClassPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Resource: {
            auto resourceDesc = reinterpret_cast<const ResourcePropertyDesc *>(desc);
            auto property = q_new<ResourceProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            property->setResourceType(resourceDesc->property);
            property->setClass(ResourceProperty::StaticClass());

            instance = property;
            metas = resourceDesc->metas;
        } break;

        case EPropertyGenFlags::Int8:
        case EPropertyGenFlags::UInt8:
            instance = q_new<Int8Property>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(Int8Property::StaticClass());
            metas = reinterpret_cast<const GenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int32:
        case EPropertyGenFlags::UInt32:
            instance = q_new<Int32Property>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(Int32Property::StaticClass());
            metas = reinterpret_cast<const GenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int64:
        case EPropertyGenFlags::UInt64:
            instance = q_new<Int64Property>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(Int64Property::StaticClass());
            metas = reinterpret_cast<const GenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Float:
            instance = q_new<FloatProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(FloatProperty::StaticClass());
            metas = reinterpret_cast<const GenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Double:
            instance = q_new<DoubleProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(DoubleProperty::StaticClass());
            metas = reinterpret_cast<const GenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::String:
            instance = q_new<StringProperty>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(StringProperty::StaticClass());
            metas = reinterpret_cast<const GenericPropertyDesc *>(desc)->metas;
            break;

        default:
            instance = q_new<Property>(target, offsetDesc->name, offsetDesc->propertyFlags, offsetDesc->offset);
            instance->setClass(Property::StaticClass());
            metas = reinterpret_cast<const GenericPropertyDesc *>(desc)->metas;
            break;
    }

    instance->setSize(desc->size);

    for (auto meta : metas) {
        instance->setMetaData(meta.name, meta.value);
    }
}

void Reflection::CreateEnum(Enum *&target, const EnumDesc &desc) {
    if (target) {
        return;
    }

    target = q_new<Enum>(Enum::StaticClass(), desc.name);
    target->setClass(Enum::StaticClass());

    TMap<String, int64_t> entries;

    for (auto entryDesc : desc.entries) {
        entries.add(entryDesc.name, entryDesc.value);
    }

    target->setEntries(entries);
    target->cppType = desc.cppType;

    for (auto meta : desc.metas) {
        target->setMetaData(meta.name, meta.value);
    }
}

void Reflection::CreateStruct(Struct *&target, const Reflection::StructDesc &desc) {
    if (target) {
        return;
    }

    target = q_new<Struct>(nullptr, desc.name, desc.size);
    target->rename(desc.name);
    target->setClass(Struct::StaticClass());

    auto &map = GetStructDescMap();
    map[target] = &desc;
}

void Reflection::CreateClass(Class *&target, const Reflection::ClassDesc &desc) {
    if (target) {
        return;
    }

    target = desc.fnNoRegister();
    target->setClass(target);

    GetClassDescMap()[target] = &desc;
}

TMap<Class *, const Reflection::ClassDesc *> &Reflection::GetClassDescMap() {
    static TMap<Class *, const ClassDesc *> instance;
    return instance;
}

TMap<Struct *, const Reflection::StructDesc *> &Reflection::GetStructDescMap() {
    static TMap<Struct *, const StructDesc *> instance;
    return instance;
}

TArray<Class *(*)()> &Reflection::GetClassRegisterList() {
    static TArray<Class *(*)()> instance;
    return instance;
}

TArray<Struct *(*)()> &Reflection::GetStructRegisterList() {
    static TArray<Struct *(*)()> instance;
    return instance;
}