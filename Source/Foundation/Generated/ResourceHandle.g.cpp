
#include "Resource/ResourceHandle.h"
#include "Reflection/GeneratedPrerequisites.h"


QStruct *Generated_Initializer_Struct_FResourceHandleData();
QStruct *FResourceHandleData::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FResourceHandleData();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FResourceHandleData(&Generated_Initializer_Struct_FResourceHandleData, &FResourceHandleData::StaticStruct, TEXT("FResourceHandleData"), TEXT("Resource/ResourceHandle.h"));


struct Generated_Struct_FResourceHandleData_Statics {
    static const QReflection::FStructPropertyDesc uuid_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> uuid_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const QReflection::FStructPropertyDesc Generated_Struct_FResourceHandleData_Statics::uuid_PropertyDesc = {
    TEXT("uuid"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 14,
    sizeof(FResourceHandleData::uuid),
    1,
    offsetof(FResourceHandleData, uuid),
    FUuid::StaticStruct,
    Generated_Struct_FResourceHandleData_Statics::uuid_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FResourceHandleData_Statics::uuid_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FResourceHandleData_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FResourceHandleData_Statics::StructProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FResourceHandleData_Statics::uuid_PropertyDesc,
};

const QReflection::FStructDesc Generated_Struct_FResourceHandleData_Statics::StructDesc = {
    TEXT("FResourceHandleData"),
    FResourceHandleData::StaticStruct,
    (EStructFlags) 0,
    sizeof(FResourceHandleData),
    Generated_Struct_FResourceHandleData_Statics::StructProperties,
    Generated_Struct_FResourceHandleData_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FResourceHandleData() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FResourceHandleData_Statics::StructDesc);
    }
    return instance;
}

QStruct *Generated_Initializer_Struct_FResourceHandleBase();
QStruct *FResourceHandleBase::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FResourceHandleBase();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FResourceHandleBase(&Generated_Initializer_Struct_FResourceHandleBase, &FResourceHandleBase::StaticStruct, TEXT("FResourceHandleBase"), TEXT("Resource/ResourceHandle.h"));


struct Generated_Struct_FResourceHandleBase_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FResourceHandleBase_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FResourceHandleBase_Statics::StructProperties = {
};

const QReflection::FStructDesc Generated_Struct_FResourceHandleBase_Statics::StructDesc = {
    TEXT("FResourceHandleBase"),
    FResourceHandleBase::StaticStruct,
    (EStructFlags) 0,
    sizeof(FResourceHandleBase),
    Generated_Struct_FResourceHandleBase_Statics::StructProperties,
    Generated_Struct_FResourceHandleBase_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FResourceHandleBase() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FResourceHandleBase_Statics::StructDesc);
    }
    return instance;
}
