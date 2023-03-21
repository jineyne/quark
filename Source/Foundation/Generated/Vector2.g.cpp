
#include "Math/Vector2.h"
#include "Reflection/GeneratedPrerequisites.h"


QStruct *Generated_Initializer_Struct_FVector2();
QStruct *FVector2::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_FVector2();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_FVector2(&Generated_Initializer_Struct_FVector2, &FVector2::StaticStruct, TEXT("FVector2"), TEXT("Math/Vector2.h"));


struct Generated_Struct_FVector2_Statics {
    static const QReflection::FGenericPropertyDesc x_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> x_MetaData;
    static const QReflection::FGenericPropertyDesc y_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> y_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const QReflection::FGenericPropertyDesc Generated_Struct_FVector2_Statics::x_PropertyDesc = {
    TEXT("x"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FVector2::x),
    1,
    offsetof(FVector2, x),
    Generated_Struct_FVector2_Statics::x_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector2_Statics::x_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Struct_FVector2_Statics::y_PropertyDesc = {
    TEXT("y"),
    (EPropertyFlags) 1,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FVector2::y),
    1,
    offsetof(FVector2, y),
    Generated_Struct_FVector2_Statics::y_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector2_Statics::y_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector2_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FVector2_Statics::StructProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FVector2_Statics::x_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Struct_FVector2_Statics::y_PropertyDesc,
};

const QReflection::FStructDesc Generated_Struct_FVector2_Statics::StructDesc = {
    TEXT("FVector2"),
    FVector2::StaticStruct,
    (EStructFlags) 0,
    sizeof(FVector2),
    Generated_Struct_FVector2_Statics::StructProperties,
    Generated_Struct_FVector2_Statics::StructMetaData,
};

QStruct *Generated_Initializer_Struct_FVector2() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_FVector2_Statics::StructDesc);
    }
    return instance;
}
