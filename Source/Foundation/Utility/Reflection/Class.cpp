#include "Class.h"

#include "Reflection/Reflection.h"

IMPLEMENT_CLASS_NO_CTR(Class)

Class::Class(size_t size, ClassConstructorType fnClassConstructor)
        : Struct(nullptr, String::Empty, size)
        , classConstructor(fnClassConstructor) {}

void Class::initProperties() {
    auto desc = Reflection::GetClassDescMap()[this];

    if (desc != nullptr) {
        for (auto propertyDesc: desc->properties) {
            Reflection::CreateProperty(this, propertyDesc);
        }
    }

    bIsPropertyInitialized = true;
}
