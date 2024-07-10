#include "Struct.h"

#include "Reflection/Class.h"
#include "Reflection/Property.h"
#include "Reflection/ObjectHash.h"
#include "Reflection/Reflection.h"

IMPLEMENT_CLASS_NO_CTR(Struct);

Struct::Struct(Struct *super, String name, size_t size)
    : Field(nullptr, name, 0), mSuperStruct(super), mSize(size) {}

void Struct::serialize(void *target, ArchiveFormatter &formatter) {
    formatter.enterRecord();

    if (formatter.isSaving()) {
        auto fields = getCppProperties();

        /*for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            formatter.enterField(name);
            property->serializeElement(target, formatter);
            formatter.leaveField();
        }*/

        for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            formatter.enterField(name);
            if ((getFlags() & PropertyFlags_Pointer) == PropertyFlags_Pointer) {
                property->serializeElement(* (void **) target, formatter);
            } else {
                property->serializeElement(target, formatter);
            }
            formatter.leaveField();
        }
    } else {
        // isLoading
        if ((getFlags() & PropertyFlags_Pointer) == PropertyFlags_Pointer) {
            size_t size = getSize();
            * (void **) target = q_alloc(size);
            memset(target, 0, size);
        }

        String fieldName = String::Empty;
        while ((formatter.enterField(fieldName), !fieldName.empty())) {
            auto field = getCppPropertiesByName(fieldName);
            if (!field) {
                formatter.leaveField();
                break;
            }

            if (!field->isA<Property>()) {
                formatter.leaveField();
                break;
            }

            auto property = (Property *) field;
            if ((getFlags() & PropertyFlags_Pointer) == PropertyFlags_Pointer) {
                property->serializeElement(* (void **) target, formatter);
            } else {
                property->serializeElement(target, formatter);
            }
            formatter.leaveField();
        }
    }

    formatter.leaveRecord();
}

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
