#include "Reflection.h"

#include "Container/Array.h"
#include "Container/String.h"

#include "Exception/Exception.h"

#include "Reflection/Class.h"
#include "Reflection/ObjectHash.h"

DEFINE_LOG_CATEGORY(LogReflection);

FString makeUniqueObjectName(QObject *parent, QClass *clazz, const FString &name) {
    assert(clazz);

    QObject *found = nullptr;
    // TODO: check name

    FString baseName = name.empty() ? clazz->getName() : name;
    FString testName;
    do {
        QObject *object;
        do {
            static std::atomic<int32_t> uniqueIdx = 0;
            testName = FString::Printf(TEXT("%ls %d"), *baseName, ++uniqueIdx);
            // testName = fmt::format("{0} {1}", baseName, ++uniqueIdx);
            object = gObjectHash().find(testName, parent);
        } while (object);
    } while (testName == baseName);

    return testName;
}

void QReflection::Initialize() {
    auto structRegisterList = std::move(GetStructRegisterList());
    if (!structRegisterList.empty()) {
        for (auto &fnRegister : structRegisterList) {
            QStruct *target = fnRegister();
        }
    }

    auto classRegisterList = std::move(GetClassRegisterList());
    if (!classRegisterList.empty()) {
        for (auto &fnRegister : classRegisterList) {
            QClass *target = fnRegister();
            gObjectHash().add(target);
        }
    }
}

QObject *QReflection::InitObject(QObject *target, QObject *parent, QClass *clazz, FString name, EObjectFlags flags) {
    assert(target != nullptr);
    // assert(parent != nullptr);

    if (name.empty()) {
        name = makeUniqueObjectName(parent, (QClass *) clazz, name);
    }

    QObject *temp = gObjectHash().find(name, parent);
    if (temp) {
        LOG(LogReflection, Warning, TEXT("%s is already in %s"), *name, *temp->getName());
        name = makeUniqueObjectName(parent, clazz, name);
    }

    target->mName = name;

    target->setClass((QClass *) clazz);

    gObjectHash().add(target);

    return target;
}

void QReflection::RegisterClass(QClass *(*fnRegister)(), QClass *(*fnStaticClass)(), const FString &name) {
    GetClassRegisterList().add(fnRegister);
}

void QReflection::RegisterStruct(QStruct *(*fnRegister)(), QStruct *(*fnStaticStruct)(), const FString &name) {
    GetStructRegisterList().add(fnRegister);
}

void QReflection::GetPrivateStaticClass(QClass *&instance, void (*fnInitNativeClass)(), QClass::ClassConstructorType fnClassConstructor, size_t size, const FString &name, QClass *(*fnSuperStaticClass)()) {
    if (gObjectHash().find(name, nullptr) != nullptr) {
        LOG(LogReflection, Fatal, TEXT("%s is duplicated class"), *name)
    }

    instance = q_new<QClass>(size, fnClassConstructor);
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

void QReflection::CreateProperty(QStruct *target, const FPropertyDescBase* desc) {
    auto offsetDesc = reinterpret_cast<const FPropertyDescBaseWithOffset *>(desc);
    QProperty *instance = nullptr;
    TArray<FMetaDataPairDesc> metas;

    switch (desc->flags) {
        case EPropertyGenFlags::Array:
        case EPropertyGenFlags::Map:
        case EPropertyGenFlags::Set: {
            auto arrayDesc = reinterpret_cast<const FArrayPropertyDesc *>(desc);
            auto property = q_new<QArrayProperty>(target, offsetDesc->name, offsetDesc->offset);
            property->setTemplateType(arrayDesc->property);
            property->setClass(QArrayProperty::StaticClass());

            instance = property;
            metas = arrayDesc->metas;
        } break;

        case EPropertyGenFlags::Object:
            instance = q_new<QObjectProperty>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QObjectProperty::StaticClass());
            metas = reinterpret_cast<const FObjectPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Struct:
            instance = q_new<QStructProperty>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QStructProperty::StaticClass());
            metas = reinterpret_cast<const FStructPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Class:
            instance = q_new<QClassProperty>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QClassProperty::StaticClass());
            metas = reinterpret_cast<const FClassPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int8:
        case EPropertyGenFlags::UInt8:
            instance = q_new<QInt8Property>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QInt8Property::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int32:
        case EPropertyGenFlags::UInt32:
            instance = q_new<QInt32Property>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QInt32Property::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int64:
        case EPropertyGenFlags::UInt64:
            instance = q_new<QInt64Property>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QInt64Property::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Float:
            instance = q_new<QFloatProperty>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QFloatProperty::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Double:
            instance = q_new<QDoubleProperty>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QDoubleProperty::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::String:
            instance = q_new<QStringProperty>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QStringProperty::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        default:
            instance = q_new<QProperty>(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QProperty::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;
    }

    instance->setSize(desc->size);

    for (auto meta : metas) {
        instance->setMetaData(meta.name, meta.value);
    }
}

void QReflection::CreateEnum(QEnum *&target, const FEnumDesc &desc) {
    if (target) {
        return;
    }

    target = q_new<QEnum>(QEnum::StaticClass(), desc.name);
    target->setClass(QEnum::StaticClass());

    TMap<FString, int64_t> entries;

    for (auto entryDesc : desc.entries) {
        entries.add(entryDesc.name, entryDesc.value);
    }

    target->setEntries(entries);
    target->cppType = desc.cppType;

    for (auto meta : desc.metas) {
        target->setMetaData(meta.name, meta.value);
    }
}

void QReflection::CreateStruct(QStruct *&target, const QReflection::FStructDesc &desc) {
    if (target) {
        return;
    }

    target = q_new<QStruct>(nullptr, desc.name, desc.size);
    target->rename(desc.name);
    target->setClass(QStruct::StaticClass());

    auto &map = GetStructDescMap();
    map[target] = &desc;
}

void QReflection::CreateClass(QClass *&target, const QReflection::FClassDesc &desc) {
    if (target) {
        return;
    }

    target = desc.fnNoRegister();
    target->setClass(target);

    GetClassDescMap()[target] = &desc;
}

TMap<QClass *, const QReflection::FClassDesc *> &QReflection::GetClassDescMap() {
    static TMap<QClass *, const FClassDesc *> instance;
    return instance;
}

TMap<QStruct *, const QReflection::FStructDesc *> &QReflection::GetStructDescMap() {
    static TMap<QStruct *, const FStructDesc *> instance;
    return instance;
}

TArray<QClass *(*)()> &QReflection::GetClassRegisterList() {
    static TArray<QClass *(*)()> instance;
    return instance;
}

TArray<QStruct *(*)()> &QReflection::GetStructRegisterList() {
    static TArray<QStruct *(*)()> instance;
    return instance;
}