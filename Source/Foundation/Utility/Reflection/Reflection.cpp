#include "Reflection.h"

#include "Container/Array.h"
#include "Container/String.h"

// #include "Error/Exception.h"

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

void QReflection::GetPrivateStaticClass(QClass *&instance, void (*fnInitNativeClass)(), size_t size, const FString &name, QClass *(*fnSuperStaticClass)()) {
    if (gObjectHash().find(name, nullptr) != nullptr) {
        LOG(LogReflection, Fatal, TEXT("%s is duplicated class"), *name)
    }

    instance = new QClass(size);
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
        case EPropertyGenFlags::Set:
            instance = new QArrayProperty(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QArrayProperty::StaticClass());
            metas = reinterpret_cast<const FArrayPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Object:
            instance = new QObjectProperty(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QObjectProperty::StaticClass());
            metas = reinterpret_cast<const FObjectPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Struct:
            instance = new QStructProperty(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QStructProperty::StaticClass());
            metas = reinterpret_cast<const FStructPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Class:
            instance = new QClassProperty(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QClassProperty::StaticClass());
            metas = reinterpret_cast<const FClassPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int8:
        case EPropertyGenFlags::UInt8:
            instance = new QInt8Property(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QInt8Property::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int32:
        case EPropertyGenFlags::UInt32:
            instance = new QInt32Property(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QInt32Property::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Int64:
        case EPropertyGenFlags::UInt64:
            instance = new QInt64Property(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QInt64Property::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Float:
            instance = new QFloatProperty(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QFloatProperty::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::Double:
            instance = new QDoubleProperty(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QDoubleProperty::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        case EPropertyGenFlags::String:
            instance = new QStringProperty(target, offsetDesc->name, offsetDesc->offset);
            instance->setClass(QStringProperty::StaticClass());
            metas = reinterpret_cast<const FGenericPropertyDesc *>(desc)->metas;
            break;

        default:
            instance = new QProperty(target, offsetDesc->name, offsetDesc->offset);
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

    target = new QEnum(QEnum::StaticClass(), desc.name);
    target->setClass(QEnum::StaticClass());

    TArray<std::pair<FString, int64_t>> entries;

    for (auto entryDesc : desc.entries) {
        entries.add(std::make_pair(entryDesc.name, entryDesc.value));
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

    target = new QStruct(nullptr, desc.name, desc.size);
    target->rename(desc.name);
    target->setClass(QStruct::StaticClass());

    for (auto propertyDesc : desc.properties) {
        CreateProperty(target, propertyDesc);
    }
}

void QReflection::CreateClass(QClass *&target, const QReflection::FClassDesc &desc) {
    if (target) {
        return;
    }

    target = desc.fnNoRegister();
    // target->setClass(QClass::StaticClass());
    target->setClass(target);

    for (auto propertyDesc : desc.properties) {
        CreateProperty(target, propertyDesc);
    }
}

TArray<QClass *(*)()> &QReflection::GetClassRegisterList() {
    static TArray<QClass *(*)()> instance;
    return instance;
}

TArray<QStruct *(*)()> &QReflection::GetStructRegisterList() {
    static TArray<QStruct *(*)()> instance;
    return instance;
}