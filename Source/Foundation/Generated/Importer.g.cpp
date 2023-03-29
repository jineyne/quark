
#include "Importer/Importer.h"
#include "Reflection/GeneratedPrerequisites.h"

void FImporter::StaticRegisterNativeFImporter() {
}
IMPLEMENT_CLASS(FImporter);
QClass *Generated_Initializer_Class_FImporter();
static FInitClassOnStart Generated_InitClassOnStart_Class_FImporter(&Generated_Initializer_Class_FImporter, &FImporter::StaticClass, TEXT("FImporter"), TEXT("Importer/Importer.h"));

struct Generated_Class_FImporter_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_FImporter_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_FImporter_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_FImporter_Statics::ClassDesc = {
    TEXT("FImporter"),
    FImporter::StaticClass,
    (EClassFlags) 0,

    Generated_Class_FImporter_Statics::ClassProperties,
    Generated_Class_FImporter_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_FImporter() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_FImporter_Statics::ClassDesc);
    }
    return instance;
}
