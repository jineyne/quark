#include "Resource/Resource.h"
#include "Reflection/GeneratedPrerequisites.h"

void FResource::StaticRegisterNativeFResource() {}
IMPLEMENT_CLASS(FResource);
QClass *Generated_Initializer_Class_FResource();
static FInitClassOnStart Generated_InitClassOnStart_Class_FResource(&Generated_Initializer_Class_FResource,
                                                                    &FResource::StaticClass, TEXT("FResource"),
                                                                    TEXT("Resource/Resource.h"));

struct Generated_Class_FResource_Statics {
    static const QReflection::FStructPropertyDesc mUuid_PropertyDesc;
    static const TArray<QReflection::FMetaDataPairDesc> mUuid_MetaData;

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<const QReflection::FPropertyDescBase*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const QReflection::FStructPropertyDesc Generated_Class_FResource_Statics::mUuid_PropertyDesc = {
    TEXT("mUuid"),
    static_cast<EPropertyFlags>(4),
    static_cast<QReflection::EPropertyGenFlags>(14),
    sizeof(FResource::mUuid),
    1,
    offsetof(FResource, mUuid),
    FUuid::StaticStruct,
    mUuid_MetaData,
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::mUuid_MetaData = {
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FResource_Statics::ClassMetaData = {
};
const TArray<const QReflection::FPropertyDescBase*> Generated_Class_FResource_Statics::ClassProperties = {
    (const QReflection::FPropertyDescBase*) &Generated_Class_FResource_Statics::mUuid_PropertyDesc,
};

const QReflection::FClassDesc Generated_Class_FResource_Statics::ClassDesc = {
    TEXT("FResource"),
    FResource::StaticClass,
    static_cast<EClassFlags>(0),

    ClassProperties,
    ClassMetaData,
};

QClass *Generated_Initializer_Class_FResource() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FResource_Statics::ClassDesc);
    }
    return instance;
}
