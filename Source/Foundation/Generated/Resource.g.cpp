
#include "Resource/Resource.h"
#include "Reflection/GeneratedPrerequisites.h"

void FResource::StaticRegisterNativeFResource() {
}
IMPLEMENT_CLASS(FResource);
QClass *Generated_Initializer_Class_FResource();
static FInitClassOnStart Generated_InitClassOnStart_Class_FResource(&Generated_Initializer_Class_FResource, &FResource::StaticClass, TEXT("FResource"), TEXT("Resource/Resource.h"));

struct Generated_Class_FResource_Statics {
    static const QReflection::FStructPropertyDesc mUuid_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mUuid_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FStructPropertyDesc Generated_Class_FResource_Statics::mUuid_PropertyDesc = {
    TEXT("mUuid"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 14,
    sizeof(FResource::mUuid),
    1,
    offsetof(FResource, mUuid),
    FUuid::StaticStruct,
    Generated_Class_FResource_Statics::mUuid_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::mUuid_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FResource_Statics::ClassProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Class_FResource_Statics::mUuid_PropertyDesc,
};

const QReflection::FClassDesc Generated_Class_FResource_Statics::ClassDesc = {
    TEXT("FResource"),
    FResource::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FResource_Statics::ClassProperties,
    Generated_Class_FResource_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FResource() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FResource_Statics::ClassDesc);
    }
    return instance;
}
