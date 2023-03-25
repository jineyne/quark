
#include "Image/Texture.h"
#include "Reflection/GeneratedPrerequisites.h"

void FTexture::StaticRegisterNativeFTexture() {
}
IMPLEMENT_CLASS_NO_CTR(FTexture);
QClass *Generated_Initializer_Class_FTexture();
static FInitClassOnStart Generated_InitClassOnStart_Class_FTexture(&Generated_Initializer_Class_FTexture, &FTexture::StaticClass, TEXT("FTexture"), TEXT("Image/Texture.h"));

struct Generated_Class_FTexture_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FTexture_Statics::ClassMetaData = {
    {TEXT("abstract"), TEXT("true")},
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FTexture_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FTexture_Statics::ClassDesc = {
    TEXT("FTexture"),
    FTexture::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FTexture_Statics::ClassProperties,
    Generated_Class_FTexture_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FTexture() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FTexture_Statics::ClassDesc);
    }
    return instance;
}
