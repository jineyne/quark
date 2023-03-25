
#include "Image/DX11Texture.h"
#include "Reflection/GeneratedPrerequisites.h"

void FDX11Texture::StaticRegisterNativeFDX11Texture() {
}
IMPLEMENT_CLASS(FDX11Texture);
QClass *Generated_Initializer_Class_FDX11Texture();
static FInitClassOnStart Generated_InitClassOnStart_Class_FDX11Texture(&Generated_Initializer_Class_FDX11Texture, &FDX11Texture::StaticClass, TEXT("FDX11Texture"), TEXT("Image/DX11Texture.h"));

struct Generated_Class_FDX11Texture_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FDX11Texture_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FDX11Texture_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FDX11Texture_Statics::ClassDesc = {
    TEXT("FDX11Texture"),
    FDX11Texture::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FDX11Texture_Statics::ClassProperties,
    Generated_Class_FDX11Texture_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FDX11Texture() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FDX11Texture_Statics::ClassDesc);
    }
    return instance;
}
