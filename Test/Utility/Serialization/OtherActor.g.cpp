
#include "OtherActor.h"
#include "Reflection/GeneratedPrerequisites.h"

void QOtherData::StaticRegisterNativeQOtherData() {
    // FUNCTION getFoo DECLARE
    // FUNCTION setFoo DECLARE
    // FUNCTION getBar DECLARE
    // FUNCTION setBar DECLARE
}

IMPLEMENT_CLASS(QOtherData);
QClass *Generated_Initializer_Class_QOtherData();
static FInitClassOnStart Generated_InitClassOnStart_Class_QOtherData(&Generated_Initializer_Class_QOtherData, &QOtherData::StaticClass, TEXT("QOtherData"), TEXT("OtherActor.h"));

struct Generated_Class_QOtherData_Statics {
    static const QReflection::FGenericPropertyDesc mFoo_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mFoo_MetaData;
    static const QReflection::FGenericPropertyDesc mBar_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mBar_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FGenericPropertyDesc Generated_Class_QOtherData_Statics::mFoo_PropertyDesc = {
    TEXT("mFoo"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 3,
    sizeof(QOtherData::mFoo),
    1,
    offsetof(QOtherData, mFoo),
    Generated_Class_QOtherData_Statics::mFoo_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_QOtherData_Statics::mFoo_MetaData = {
};

const QReflection::FGenericPropertyDesc Generated_Class_QOtherData_Statics::mBar_PropertyDesc = {
    TEXT("mBar"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 3,
    sizeof(QOtherData::mBar),
    1,
    offsetof(QOtherData, mBar),
    Generated_Class_QOtherData_Statics::mBar_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_QOtherData_Statics::mBar_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_QOtherData_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_QOtherData_Statics::ClassProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Class_QOtherData_Statics::mFoo_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Class_QOtherData_Statics::mBar_PropertyDesc,
};

const QReflection::FClassDesc Generated_Class_QOtherData_Statics::ClassDesc = {
    TEXT("QOtherData"),
    QOtherData::StaticClass,
    (EClassFlags) 0,

    Generated_Class_QOtherData_Statics::ClassProperties,
    Generated_Class_QOtherData_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_QOtherData() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_QOtherData_Statics::ClassDesc);
    }
    return instance;
}


// CLASS QOtherData DECLARE END

