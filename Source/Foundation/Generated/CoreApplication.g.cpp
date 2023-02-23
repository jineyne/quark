
#include "CoreApplication.h"
#include "Reflection/GeneratedPrerequisites.h"

void QCoreApplication::StaticRegisterNativeQCoreApplication() {
}
IMPLEMENT_CLASS(QCoreApplication);
QClass *Generated_Initializer_Class_QCoreApplication();
static FInitClassOnStart Generated_InitClassOnStart_Class_QCoreApplication(&Generated_Initializer_Class_QCoreApplication, &QCoreApplication::StaticClass, TEXT("QCoreApplication"), TEXT("CoreApplication.h"));

struct Generated_Class_QCoreApplication_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_QCoreApplication_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_QCoreApplication_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_QCoreApplication_Statics::ClassDesc = {
    TEXT("QCoreApplication"),
    QCoreApplication::StaticClass,
    (EClassFlags) 0,

    Generated_Class_QCoreApplication_Statics::ClassProperties,
    Generated_Class_QCoreApplication_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_QCoreApplication() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_QCoreApplication_Statics::ClassDesc);
    }
    return instance;
}
