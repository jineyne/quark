#include "Struct.h"

#include "Reflection/Class.h"
#include "Reflection/Property.h"
#include "Reflection/ObjectHash.h"
#include "Reflection/Reflection.h"

IMPLEMENT_CLASS(QStruct);

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

TArray<QField *> QStruct::getCppProperties(bool deepSearch) const {
    TArray<QField *> fields = mChildProperties;

    if (deepSearch && mSuperStruct != nullptr) {
        fields.append(mSuperStruct->getCppProperties(deepSearch));
    }

    return fields;
}
