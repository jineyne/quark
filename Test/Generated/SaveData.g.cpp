
#include "Serialization/SaveData.h"
#include "Reflection/GeneratedPrerequisites.h"


QStruct *Generated_Initializer_Struct_FSaveData();
QStruct *FSaveData::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FSaveData();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FSaveData(&Generated_Initializer_Struct_FSaveData, &FSaveData::StaticStruct, TEXT("FSaveData"), TEXT("Serialization/SaveData.h"));


struct Generated_Struct_FSaveData_Statics {
    static const QReflection::FGenericPropertyDesc level_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> level_MetaData;
    static const QReflection::FGenericPropertyDesc coin_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> coin_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const QReflection::FGenericPropertyDesc Generated_Struct_FSaveData_Statics::level_PropertyDesc = {
    TEXT("level"),
    (EPropertyFlags) 0,
    (QReflection::EPropertyGenFlags) 3,
    sizeof(FSaveData::level),
    1,
    offsetof(FSaveData, level),
    Generated_Struct_FSaveData_Statics::level_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FSaveData_Statics::level_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Struct_FSaveData_Statics::coin_PropertyDesc = {
    TEXT("coin"),
    (EPropertyFlags) 0,
    (QReflection::EPropertyGenFlags) 3,
    sizeof(FSaveData::coin),
    1,
    offsetof(FSaveData, coin),
    Generated_Struct_FSaveData_Statics::coin_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FSaveData_Statics::coin_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FSaveData_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FSaveData_Statics::StructProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FSaveData_Statics::level_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FSaveData_Statics::coin_PropertyDesc,
};

const QReflection::FStructDesc Generated_Struct_FSaveData_Statics::StructDesc = {
    TEXT("FSaveData"),
    FSaveData::StaticStruct,
    (EStructFlags) 0,
    sizeof(FSaveData),
    Generated_Struct_FSaveData_Statics::StructProperties,
    Generated_Struct_FSaveData_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FSaveData() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FSaveData_Statics::StructDesc);
    }
    return instance;
}
