
#include "Scene/Transform.h"
#include "Reflection/GeneratedPrerequisites.h"


QStruct *Generated_Initializer_Struct_FTransform();
QStruct *FTransform::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FTransform();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FTransform(&Generated_Initializer_Struct_FTransform, &FTransform::StaticStruct, TEXT("FTransform"), TEXT("Scene/Transform.h"));


struct Generated_Struct_FTransform_Statics {
    static const QReflection::FStructPropertyDesc mPosition_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mPosition_MetaData;
    static const QReflection::FStructPropertyDesc mRotation_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mRotation_MetaData;
    static const QReflection::FStructPropertyDesc mScale_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mScale_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const QReflection::FStructPropertyDesc Generated_Struct_FTransform_Statics::mPosition_PropertyDesc = {
    TEXT("mPosition"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 14,
    sizeof(FTransform::mPosition),
    1,
    offsetof(FTransform, mPosition),
    FVector3::StaticStruct,
    Generated_Struct_FTransform_Statics::mPosition_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FTransform_Statics::mPosition_MetaData = {
};

const QReflection::FStructPropertyDesc Generated_Struct_FTransform_Statics::mRotation_PropertyDesc = {
    TEXT("mRotation"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 14,
    sizeof(FTransform::mRotation),
    1,
    offsetof(FTransform, mRotation),
    FQuaternion::StaticStruct,
    Generated_Struct_FTransform_Statics::mRotation_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FTransform_Statics::mRotation_MetaData = {
};

const QReflection::FStructPropertyDesc Generated_Struct_FTransform_Statics::mScale_PropertyDesc = {
    TEXT("mScale"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 14,
    sizeof(FTransform::mScale),
    1,
    offsetof(FTransform, mScale),
    FVector3::StaticStruct,
    Generated_Struct_FTransform_Statics::mScale_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FTransform_Statics::mScale_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FTransform_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FTransform_Statics::StructProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FTransform_Statics::mPosition_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FTransform_Statics::mRotation_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FTransform_Statics::mScale_PropertyDesc,
};

const QReflection::FStructDesc Generated_Struct_FTransform_Statics::StructDesc = {
    TEXT("FTransform"),
    FTransform::StaticStruct,
    (EStructFlags) 0,
    sizeof(FTransform),
    Generated_Struct_FTransform_Statics::StructProperties,
    Generated_Struct_FTransform_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FTransform() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FTransform_Statics::StructDesc);
    }
    return instance;
}
