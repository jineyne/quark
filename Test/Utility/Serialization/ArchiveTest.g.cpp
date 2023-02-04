
#include "ArchiveTest.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDerivedClass::StaticRegisterNativeFDerivedClass() {
    // FUNCTION dump DECLARE
}

IMPLEMENT_CLASS(FDerivedClass);
QClass *Generated_Initializer_Class_FDerivedClass();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDerivedClass(&Generated_Initializer_Class_FDerivedClass, &FDerivedClass::StaticClass, TEXT("FDerivedClass"), TEXT("ArchiveTest.h"));

struct Generated_Class_FDerivedClass_Statics {
    static const QReflection::FGenericPropertyDesc mFloatValue_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mFloatValue_MetaData;
    static const QReflection::FArrayPropertyDesc mOtherDataList_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mOtherDataList_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FGenericPropertyDesc Generated_Class_FDerivedClass_Statics::mFloatValue_PropertyDesc = {
    TEXT("mFloatValue"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 8,
    sizeof(FDerivedClass::mFloatValue),
    1,
    offsetof(FDerivedClass, mFloatValue),
    Generated_Class_FDerivedClass_Statics::mFloatValue_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDerivedClass_Statics::mFloatValue_MetaData = {
};

const QReflection::FArrayPropertyDesc Generated_Class_FDerivedClass_Statics::mOtherDataList_PropertyDesc = {
    TEXT("mOtherDataList"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 16,
    sizeof(FDerivedClass::mOtherDataList),
    1,
    offsetof(FDerivedClass, mOtherDataList),
    new QClassProperty(QOtherData::StaticClass(), TEXT("FDerivedClass_QOtherData_Template"), 0),
    Generated_Class_FDerivedClass_Statics::mOtherDataList_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDerivedClass_Statics::mOtherDataList_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDerivedClass_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDerivedClass_Statics::ClassProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Class_FDerivedClass_Statics::mFloatValue_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Class_FDerivedClass_Statics::mOtherDataList_PropertyDesc,
};

const QReflection::FClassDesc Generated_Class_FDerivedClass_Statics::ClassDesc = {
    TEXT("FDerivedClass"),
    FDerivedClass::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDerivedClass_Statics::ClassProperties,
    Generated_Class_FDerivedClass_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDerivedClass() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDerivedClass_Statics::ClassDesc);
    }
    return instance;
}


// CLASS FDerivedClass DECLARE END

