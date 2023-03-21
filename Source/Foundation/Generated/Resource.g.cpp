
#include "Resource/Resource.h"
#include "Reflection/GeneratedPrerequisites.h"

void FResource::StaticRegisterNativeFResource() {
}
IMPLEMENT_CLASS(FResource);
QClass *Generated_Initializer_Class_FResource();
static FInitClassOnStart Generated_InitClassOnStart_Class_FResource(&Generated_Initializer_Class_FResource, &FResource::StaticClass, TEXT("FResource"), TEXT("Resource/Resource.h"));

struct Generated_Class_FResource_Statics {
    static const QReflection::FGenericPropertyDesc mName_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mName_MetaData;
    static const QReflection::FStructPropertyDesc mPath_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mPath_MetaData;
    static const QReflection::FStructPropertyDesc mUuid_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mUuid_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FGenericPropertyDesc Generated_Class_FResource_Statics::mName_PropertyDesc = {
    TEXT("mName"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 11,
    sizeof(FResource::mName),
    1,
    offsetof(FResource, mName),
    Generated_Class_FResource_Statics::mName_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::mName_MetaData = {
};

const QReflection::FStructPropertyDesc Generated_Class_FResource_Statics::mPath_PropertyDesc = {
    TEXT("mPath"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 14,
    sizeof(FResource::mPath),
    1,
    offsetof(FResource, mPath),
    nullptr,
    Generated_Class_FResource_Statics::mPath_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::mPath_MetaData = {
};

const QReflection::FStructPropertyDesc Generated_Class_FResource_Statics::mUuid_PropertyDesc = {
    TEXT("mUuid"),
    (EPropertyFlags) 4,
    (QReflection::EPropertyGenFlags) 14,
    sizeof(FResource::mUuid),
    1,
    offsetof(FResource, mUuid),
    nullptr,
    Generated_Class_FResource_Statics::mUuid_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::mUuid_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FResource_Statics::ClassProperties = {
    (const QReflection::FPropertyDescBase *) &Generated_Class_FResource_Statics::mName_PropertyDesc,
    (const QReflection::FPropertyDescBase *) &Generated_Class_FResource_Statics::mPath_PropertyDesc,
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
