#include "Struct.h"

#include "Reflection/Class.h"
#include "Reflection/Property.h"
#include "Reflection/ObjectHash.h"
#include "Reflection/Reflection.h"

IMPLEMENT_CLASS_NO_CTR(Struct);

Struct::Struct(Struct *super, String name, size_t size) : Field(nullptr, name), mSuperStruct(super), mSize(size) {}

bool Struct::isChildOf(Struct *base) const {
    if (base == nullptr) {
        return false;
    }

    if (base == this) {
        return true;
    }

    const Struct *super = getSuperStruct();
    while (super != nullptr) {
        if (super == base) {
            return true;
        }

        super = super->getSuperStruct();
    }

    return false;
}

void Struct::setSuperStruct(Struct* newStruct) {
    gObjectHash().remove(this);
    mSuperStruct = newStruct;
    gObjectHash().add(this);
}

void Struct::addCppProperty(struct Property *property) {
    mChildProperties.add(property);
}

Field *Struct::getCppPropertiesByName(const String &name, bool deepSearch) {
    if (!bIsPropertyInitialized) {
        initProperties();
    }

    for (auto property : mChildProperties) {
        if (property->getName() == name) {
            return property;
        }
    }

    if (deepSearch && mSuperStruct != nullptr) {
        return mSuperStruct->getCppPropertiesByName(name, deepSearch);
    }

    return nullptr;
}

TArray<Field *> Struct::getCppProperties(bool deepSearch) {
    if (!bIsPropertyInitialized) {
        initProperties();
    }

    TArray<Field *> fields = mChildProperties;

    if (deepSearch && mSuperStruct != nullptr) {
        fields.append(mSuperStruct->getCppProperties(deepSearch));
    }

    return fields;
}

void Struct::initProperties() {
    auto desc = Reflection::GetStructDescMap()[this];

    if (desc != nullptr) {
        for (auto propertyDesc: desc->properties) {
            Reflection::CreateProperty(this, propertyDesc);
        }
    }

    bIsPropertyInitialized = true;
}
