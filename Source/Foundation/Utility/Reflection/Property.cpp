#include "Property.h"

#include "Reflection/Class.h"
#include "Reflection/Reflection.h"
#include "ObjectHash.h"

IMPLEMENT_CLASS_NO_CTR(QProperty)

QProperty::QProperty(QStruct *owner, const FString &name, size_t offset)
        : QField(QProperty::StaticClass(), name), mOwner(owner), mOffset(offset) {
    if (owner) {
        owner->addCppProperty(this);
    }
}

#define SERIALIZER(TYPE) if (ar.isSaving()) { ar << *getRawValuePtr<TYPE>(target); } else { TYPE value = 0; ar << value; setRawValuePtr(target, value); }
#define COPYTO(TYPE) setRawValuePtr(dest, getRawValuePtr<TYPE>(source))

IMPLEMENT_CLASS_NO_CTR(QNumbericProperty)
IMPLEMENT_CLASS_NO_CTR(QBoolProperty)

void QBoolProperty::serializeElement(void *target, FArchive &ar) {
    SERIALIZER(bool);
}

QBoolProperty::QBoolProperty(QStruct *target, const FString &name, uint64_t offset)
    : QNumbericProperty(target, name, offset) {
    setSize(sizeof(bool));
}

void QBoolProperty::copyTo(void *dest, void *source) {
    COPYTO(bool);
}

IMPLEMENT_CLASS_NO_CTR(QIntProperty)

void QIntProperty::serializeElement(void *target, FArchive &ar) {
    SERIALIZER(int);
}

QIntProperty::QIntProperty(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {
    setSize(sizeof(int));
}

void QIntProperty::copyTo(void *dest, void *source) {
    COPYTO(int);
}

IMPLEMENT_CLASS_NO_CTR(QInt8Property)

void QInt8Property::serializeElement(void *target, FArchive &ar) {
    SERIALIZER(int8_t);
}

QInt8Property::QInt8Property(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {
    setSize(sizeof(int8_t));
}

void QInt8Property::copyTo(void *dest, void *source) {
    COPYTO(int8_t);
}

IMPLEMENT_CLASS_NO_CTR(QInt32Property)

void QInt32Property::serializeElement(void *target, FArchive &ar) {
    SERIALIZER(int32_t);
}

QInt32Property::QInt32Property(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {
    setSize(sizeof(int32_t));
}

void QInt32Property::copyTo(void *dest, void *source) {
    COPYTO(int32_t);
}

IMPLEMENT_CLASS_NO_CTR(QInt64Property)

void QInt64Property::serializeElement(void *target, FArchive &ar) {
    SERIALIZER(int64_t);
}

QInt64Property::QInt64Property(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {
    setSize(sizeof(int64_t));
}

void QInt64Property::copyTo(void *dest, void *source) {
    COPYTO(int64_t);
}

IMPLEMENT_CLASS_NO_CTR(QFloatProperty)

void QFloatProperty::serializeElement(void *target, FArchive &ar) {
    SERIALIZER(float);
}

QFloatProperty::QFloatProperty(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {
    setSize(sizeof(float));
}

void QFloatProperty::copyTo(void *dest, void *source) {
    COPYTO(float);
}

IMPLEMENT_CLASS_NO_CTR(QDoubleProperty)

void QDoubleProperty::serializeElement(void *target, FArchive &ar) {
    SERIALIZER(double);
}

QDoubleProperty::QDoubleProperty(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {
    setSize(sizeof(double));
}

void QDoubleProperty::copyTo(void *dest, void *source) {
    COPYTO(double);
}

IMPLEMENT_CLASS_NO_CTR(QObjectProperty)

QObjectProperty::QObjectProperty(QStruct *target, const FString &name, uint64_t offset) : QProperty(nullptr, name, offset), mTarget(target) { }

QStruct *QObjectProperty::getTarget() const {
    return mTarget;
}

IMPLEMENT_CLASS_NO_CTR(QStructProperty)

QStructProperty::QStructProperty(QStruct *target, const FString &name, uint64_t offset) : QObjectProperty(target, name, offset) {
}

void QStructProperty::serializeElement(void *target, FArchive &ar) {
    if (ar.isSaving()) {
        auto fields = getTarget()->getCppProperties();

        size_t length = fields.length();
        ar << length;

        for (auto field : fields) {
            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            FString &name = const_cast<FString &>(property->getName());

            ar << name;
            property->serializeElement(target, ar);
        }

    } else {
        size_t length = 0;
        ar << length;

        for (int i = 0; i < length; i++) {
            FString name;
            ar << name;
            if (name.empty()) {
                break;
            }

            auto field = getTarget()->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            property->serializeElement(target, ar);
        }
    }
}

void QStructProperty::copyTo(void *dest, void *source) {
    auto fields = getTarget()->getCppProperties();

    for (auto field : fields) {
        if (!field->isA<QProperty>()) {
            continue;
        }

        auto property = (QProperty *) field;
        FString &name = const_cast<FString &>(property->getName());

        ((QProperty *) field)->copyTo(dest, source);
    }
}

const size_t &QStructProperty::getSize() {
    if (QProperty::getSize() == 0) {
        auto properties = getTarget()->getCppProperties();
        size_t size = 0;

        for (auto &property : properties) {
            if (property->isA<QProperty>()) {
                size += ((QProperty *) property)->getSize();
            } else if (property->isA<QStruct>()) {
                size += ((QStruct *) property)->getSize();
            } else {
                LOG(LogReflection, Warning, TEXT("Unable to find size from type '%s'"), *property->getName());
            }
        }

        QProperty::setSize(size);
    }

    return QProperty::getSize();
}



IMPLEMENT_CLASS_NO_CTR(QClassProperty)

QClassProperty::QClassProperty(QStruct *target, const FString &name, uint64_t offset) : QObjectProperty(target, name, offset) {
}

void QClassProperty::serializeElement(void *target, FArchive &ar) {
    void **ptr = (void **) target;
    if (ar.isSaving()) {
        auto fields = getTarget()->getCppProperties();

        size_t length = fields.length();
        ar << length;

        for (auto field : fields) {
            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            FString &name = const_cast<FString &>(property->getName());

            ar << name;
            property->serializeElement(*ptr, ar);
        }
    } else {
        size_t size = getTarget()->getSize();
        *ptr = malloc(size);

        size_t length = 0;
        ar << length;

        ((QClass *) getTarget())->classConstructor(*ptr);

        for (int i = 0; i < length; i++) {
            FString name;
            ar << name;
            if (name.empty()) {
                break;
            }

            auto field = getTarget()->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            property->serializeElement(*ptr, ar);
        }
    }
}

void QClassProperty::copyTo(void *dest, void *source) {
    auto fields = getTarget()->getCppProperties();

    for (auto field : fields) {
        if (!field->isA<QProperty>()) {
            continue;
        }

        auto property = (QProperty *) field;
        FString &name = const_cast<FString &>(property->getName());

        ((QProperty *) field)->copyTo(dest, source);
    }
}

const size_t &QClassProperty::getSize() {
    if (QProperty::getSize() == 0) {
        auto properties = getTarget()->getCppProperties();
        size_t size = 0;

        for (auto &property : properties) {
            if (property->isA<QProperty>()) {
                size += ((QProperty *) property)->getSize();
            } else if (property->isA<QStruct>()) {
                size += ((QStruct *) property)->getSize();
            } else {
                LOG(LogReflection, Warning, TEXT("Unable to find size from type '%s'"), *property->getName());
            }
        }

        QProperty::setSize(size);
    }

    return QProperty::getSize();
}

IMPLEMENT_CLASS_NO_CTR(QArrayProperty)

void QArrayProperty::serializeElement(void *target, FArchive &ar) {
    if (ar.isSaving()) {
        TArray<uint8_t> &array = *getRawValuePtr<TArray<uint8_t>>(target);
        size_t length = array.length();
        uint8_t *data = array.getData();

        ar << length;
        for (auto index = 0; index < length;) {
            mTemplateType->serializeElement(data, ar);
            data += mTemplateType->getSize();
            index += mTemplateType->getSize();
        }
    } else {
        size_t length = 0;
        ar << length;

        TArray<uint8_t> &array = *getRawValuePtr<TArray<uint8_t>>(target);
        array.resize(length);
        uint8_t *data = array.getData();

        for (auto index = 0; index < length;) {
            mTemplateType->serializeElement(data, ar);

            data += mTemplateType->getSize();
            index += mTemplateType->getSize();
        }
    }
}

QArrayProperty::QArrayProperty(QStruct *target, const FString &name, uint64_t offset) : QProperty(target, name, offset) {
    setSize(sizeof(TArray<uint8_t>));
}

void QArrayProperty::copyTo(void *dest, void *source) {
    QProperty::copyTo(dest, source);
}

IMPLEMENT_CLASS_NO_CTR(QStringProperty)

void QStringProperty::serializeElement(void *target, FArchive &ar) {
    if (ar.isSaving()) {
        ar << *getRawValuePtr<FString>(target);
    } else {
        new (((uint8_t *) target) + getOffset()) FString();

        FString value = TEXT("");
        ar << value;
        setRawValuePtr(target, value);
    }
}

QStringProperty::QStringProperty(QStruct *target, const FString &name, uint64_t offset) : QProperty(target, name, offset) {
    setSize(sizeof(FString));
}

void QStringProperty::copyTo(void *dest, void *source) {
    COPYTO(FString);
}
