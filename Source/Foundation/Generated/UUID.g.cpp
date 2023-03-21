
#include "Misc/UUID.h"
#include "Reflection/GeneratedPrerequisites.h"


QStruct *Generated_Initializer_Struct_FUuid();
QStruct *FUuid::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FUuid();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FUuid(&Generated_Initializer_Struct_FUuid, &FUuid::StaticStruct, TEXT("FUuid"), TEXT("Misc/UUID.h"));


struct Generated_Struct_FUuid_Statics {
    static const QReflection::FGenericPropertyDesc mData_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mData_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const QReflection::FGenericPropertyDesc Generated_Struct_FUuid_Statics::mData_PropertyDesc = {
    TEXT("mData"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 0,
    sizeof(FUuid::mData),
    1,
    offsetof(FUuid, mData),
    Generated_Struct_FUuid_Statics::mData_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FUuid_Statics::mData_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FUuid_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FUuid_Statics::StructProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FUuid_Statics::mData_PropertyDesc,
};

const QReflection::FStructDesc Generated_Struct_FUuid_Statics::StructDesc = {
    TEXT("FUuid"),
    FUuid::StaticStruct,
    (EStructFlags) 0,
    sizeof(FUuid),
    Generated_Struct_FUuid_Statics::StructProperties,
    Generated_Struct_FUuid_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FUuid() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FUuid_Statics::StructDesc);
    }
    return instance;
}
