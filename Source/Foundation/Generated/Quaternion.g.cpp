
#include "Math/Quaternion.h"
#include "Reflection/GeneratedPrerequisites.h"


QStruct *Generated_Initializer_Struct_FQuaternion();
QStruct *FQuaternion::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FQuaternion();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FQuaternion(&Generated_Initializer_Struct_FQuaternion, &FQuaternion::StaticStruct, TEXT("FQuaternion"), TEXT("Math/Quaternion.h"));


struct Generated_Struct_FQuaternion_Statics {
    static const QReflection::FGenericPropertyDesc x_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> x_MetaData;
    static const QReflection::FGenericPropertyDesc y_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> y_MetaData;
    static const QReflection::FGenericPropertyDesc z_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> z_MetaData;
    static const QReflection::FGenericPropertyDesc w_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> w_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const QReflection::FGenericPropertyDesc Generated_Struct_FQuaternion_Statics::x_PropertyDesc = {
    TEXT("x"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FQuaternion::x),
    1,
    offsetof(FQuaternion, x),
    Generated_Struct_FQuaternion_Statics::x_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FQuaternion_Statics::x_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Struct_FQuaternion_Statics::y_PropertyDesc = {
    TEXT("y"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FQuaternion::y),
    1,
    offsetof(FQuaternion, y),
    Generated_Struct_FQuaternion_Statics::y_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FQuaternion_Statics::y_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Struct_FQuaternion_Statics::z_PropertyDesc = {
    TEXT("z"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FQuaternion::z),
    1,
    offsetof(FQuaternion, z),
    Generated_Struct_FQuaternion_Statics::z_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FQuaternion_Statics::z_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Struct_FQuaternion_Statics::w_PropertyDesc = {
    TEXT("w"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FQuaternion::w),
    1,
    offsetof(FQuaternion, w),
    Generated_Struct_FQuaternion_Statics::w_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FQuaternion_Statics::w_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FQuaternion_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FQuaternion_Statics::StructProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FQuaternion_Statics::x_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FQuaternion_Statics::y_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FQuaternion_Statics::z_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FQuaternion_Statics::w_PropertyDesc,
};

const QReflection::FStructDesc Generated_Struct_FQuaternion_Statics::StructDesc = {
    TEXT("FQuaternion"),
    FQuaternion::StaticStruct,
    (EStructFlags) 0,
    sizeof(FQuaternion),
    Generated_Struct_FQuaternion_Statics::StructProperties,
    Generated_Struct_FQuaternion_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FQuaternion() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FQuaternion_Statics::StructDesc);
    }
    return instance;
}
