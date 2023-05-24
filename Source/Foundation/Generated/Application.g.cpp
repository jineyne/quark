
#include "Application.h"
#include "Reflection/GeneratedPrerequisites.h"

void QApplication::StaticRegisterNativeQApplication() {
}
IMPLEMENT_CLASS(QApplication);
QClass *Generated_Initializer_Class_QApplication();
static FInitClassOnStart Generated_InitClassOnStart_Class_QApplication(&Generated_Initializer_Class_QApplication, &QApplication::StaticClass, TEXT("QApplication"), TEXT("Application.h"));

struct Generated_Class_QApplication_Statics {

    static const TArray<QReflection::FMetaDataPairDesc> ClassMetaData;
    static const TArray<QReflection::FPropertyDescBase const*> ClassProperties;
    static const QReflection::FClassDesc ClassDesc;
};

const TArray<QReflection::FMetaDataPairDesc> Generated_Class_QApplication_Statics::ClassMetaData = {
};
const TArray<QReflection::FPropertyDescBase const*> Generated_Class_QApplication_Statics::ClassProperties = {
};

const QReflection::FClassDesc Generated_Class_QApplication_Statics::ClassDesc = {
    TEXT("QApplication"),
    QApplication::StaticClass,
    (EClassFlags) 0,

    Generated_Class_QApplication_Statics::ClassProperties,
    Generated_Class_QApplication_Statics::ClassMetaData,
};

QClass *Generated_Initializer_Class_QApplication() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_QApplication_Statics::ClassDesc);
    }
    return instance;
}
