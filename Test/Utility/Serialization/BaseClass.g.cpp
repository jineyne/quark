
#include "BaseClass.h"
#include "Reflection/GeneratedPrerequisites.h"

void FBaseClass::StaticRegisterNativeFBaseClass() {
    // FUNCTION dump DECLARE
}

IMPLEMENT_CLASS(FBaseClass);
QClass *Generated_Initializer_Class_FBaseClass();
static FInitClassOnStart Generated_InitClassOnStart_Class_FBaseClass(&Generated_Initializer_Class_FBaseClass, &FBaseClass::StaticClass, TEXT("FBaseClass"), TEXT("BaseClass.h"));

struct Generated_Class_FBaseClass_Statics {
    static const QReflection::FGenericPropertyDesc mParam1_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam1_MetaData;
    static const QReflection::FGenericPropertyDesc mParam2_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mParam2_MetaData;
    static const QReflection::FArrayPropertyDesc mArray_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mArray_MetaData;
    static const QReflection::FArrayPropertyDesc mSaveDataList_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mSaveDataList_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FGenericPropertyDesc Generated_Class_FBaseClass_Statics::mParam1_PropertyDesc = {
    TEXT("mParam1"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 3,
    sizeof(FBaseClass::mParam1),
    1,
    offsetof(FBaseClass, mParam1),
    Generated_Class_FBaseClass_Statics::mParam1_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FBaseClass_Statics::mParam1_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Class_FBaseClass_Statics::mParam2_PropertyDesc = {
    TEXT("mParam2"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 11,
    sizeof(FBaseClass::mParam2),
    1,
    offsetof(FBaseClass, mParam2),
    Generated_Class_FBaseClass_Statics::mParam2_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FBaseClass_Statics::mParam2_MetaData = {
    {TEXT("Server"), TEXT("")},
};

const QReflection::FArrayPropertyDesc Generated_Class_FBaseClass_Statics::mArray_PropertyDesc = {
    TEXT("mArray"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 16,
    sizeof(FBaseClass::mArray),
    1,
    offsetof(FBaseClass, mArray),
    new QStringProperty(nullptr, TEXT("FBaseClass_FString_Template"), 0),
    Generated_Class_FBaseClass_Statics::mArray_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FBaseClass_Statics::mArray_MetaData = {
};

const QReflection::FArrayPropertyDesc Generated_Class_FBaseClass_Statics::mSaveDataList_PropertyDesc = {
    TEXT("mSaveDataList"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 16,
    sizeof(FBaseClass::mSaveDataList),
    1,
    offsetof(FBaseClass, mSaveDataList),
    new QStructProperty(FSaveData::StaticStruct(), TEXT("FBaseClass_FSaveData_Template"), 0),
    Generated_Class_FBaseClass_Statics::mSaveDataList_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FBaseClass_Statics::mSaveDataList_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FBaseClass_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FBaseClass_Statics::ClassProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Class_FBaseClass_Statics::mParam1_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Class_FBaseClass_Statics::mParam2_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Class_FBaseClass_Statics::mArray_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Class_FBaseClass_Statics::mSaveDataList_PropertyDesc,
};

const QReflection::FClassDesc Generated_Class_FBaseClass_Statics::ClassDesc = {
    TEXT("FBaseClass"),
    FBaseClass::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FBaseClass_Statics::ClassProperties,
    Generated_Class_FBaseClass_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FBaseClass() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FBaseClass_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FBaseClass DECLARE END

