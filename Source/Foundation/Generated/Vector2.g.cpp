
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

    static const TArray<QReflection::FMetaDataPairDesc> StructMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> StructProperties;
    static const QReflection::FStructDesc StructDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Struct_FVector2_Statics::StructMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Struct_FVector2_Statics::StructProperties = {
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