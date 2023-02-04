#include "Class.h"

#include "Reflection/Reflection.h"

IMPLEMENT_CLASS_NO_CTR(QClass)

QClass::QClass(size_t size, ClassConstructorType fnClassConstructor)
        : QStruct(nullptr, FString::Empty, size)
        , classConstructor(fnClassConstructor) {}

void QClass::initProperties() {
    auto desc = QReflection::GetClassDescMap()[this];

    if (desc != nullptr) {
        for (auto propertyDesc: desc->properties) {
            QReflection::CreateProperty(this, propertyDesc);
        }
    }

    bIsPropertyInitialized = true;
}
