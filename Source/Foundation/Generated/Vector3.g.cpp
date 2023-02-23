
#include "Math/Vector3.h"
#include "Reflection/GeneratedPrerequisites.h"


QStruct *Generated_Initializer_Struct_FVector3();
QStruct *FVector3::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FVector3();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FVector3(&Generated_Initializer_Struct_FVector3, &FVector3::StaticStruct, TEXT("FVector3"), TEXT("Math/Vector3.h"));


struct Generated_Struct_FVector3_Statics {
    static const QReflection::FGenericPropertyDesc x_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> x_MetaData;
    static const QReflection::FGenericPropertyDesc y_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> y_MetaData;
    static const QReflection::FGenericPropertyDesc z_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> z_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const QReflection::FGenericPropertyDesc Generated_Struct_FVector3_Statics::x_PropertyDesc = {
    TEXT("x"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FVector3::x),
    1,
    offsetof(FVector3, x),
    Generated_Struct_FVector3_Statics::x_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector3_Statics::x_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Struct_FVector3_Statics::y_PropertyDesc = {
    TEXT("y"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FVector3::y),
    1,
    offsetof(FVector3, y),
    Generated_Struct_FVector3_Statics::y_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector3_Statics::y_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Struct_FVector3_Statics::z_PropertyDesc = {
    TEXT("z"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FVector3::z),
    1,
    offsetof(FVector3, z),
    Generated_Struct_FVector3_Statics::z_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector3_Statics::z_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector3_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FVector3_Statics::StructProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FVector3_Statics::x_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FVector3_Statics::y_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FVector3_Statics::z_PropertyDesc,
};

const QReflection::FStructDesc Generated_Struct_FVector3_Statics::StructDesc = {
    TEXT("FVector3"),
    FVector3::StaticStruct,
    (EStructFlags) 0,
    sizeof(FVector3),
    Generated_Struct_FVector3_Statics::StructProperties,
    Generated_Struct_FVector3_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FVector3() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FVector3_Statics::StructDesc);
    }
    return instance;
}
