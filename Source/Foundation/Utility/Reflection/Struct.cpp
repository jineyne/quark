#include "Struct.h"

#include "Reflection/Class.h"
#include "Reflection/Property.h"
#include "Reflection/ObjectHash.h"
#include "Reflection/Reflection.h"

IMPLEMENT_CLASS_NO_CTR(QStruct);

QStruct::QStruct(QStruct *super, FString name, size_t size) : QField(nullptr, name), mSuperStruct(super), mSize(size) {}

bool QStruct::isChildOf(QStruct *base) const {
    if (base == nullptr) {
        return false;
    }

    if (base == this) {
        return true;
    }

    const QStruct *super = getSuperStruct();
    while (super != nullptr) {
        if (super == base) {
            return true;
        }

        super = super->getSuperStruct();
    }

    return false;
}

void QStruct::setSuperStruct(QStruct* newStruct) {
    gObjectHash().remove(this);
    mSuperStruct = newStruct;
    gObjectHash().add(this);
}

void QStruct::addCppProperty(struct QProperty *property) {
    mChildProperties.add(property);
}

QField *QStruct::getCppPropertiesByName(const FString &name, bool deepSearch) {
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

TArray<QField *> QStruct::getCppProperties(bool deepSearch) {
    if (!bIsPropertyInitialized) {
        initProperties();
    }

    TArray<QField *> fields = mChildProperties;

    if (deepSearch && mSuperStruct != nullptr) {
        fields.append(mSuperStruct->getCppProperties(deepSearch));
    }

    return fields;
}

void QStruct::initProperties() {
    auto desc = QReflection::GetStructDescMap()[this];

    if (desc != nullptr) {
        for (auto propertyDesc: desc->properties) {
            QReflection::CreateProperty(this, propertyDesc);
        }
    }

    bIsPropertyInitialized = true;
}
