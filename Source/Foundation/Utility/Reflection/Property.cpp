#include "Property.h"

#include "Reflection/Class.h"
#include "Reflection/Reflection.h"
#include "ObjectHash.h"

IMPLEMENT_CLASS_NO_CTR(Property)

Property::Property(Struct *owner, const String &name, size_t offset)
        : Field(Property::StaticClass(), name), mOwner(owner), mOffset(offset) {
    if (owner) {
        owner->addCppProperty(this);
    }
}

#define SERIALIZER(TYPE) if (ar.isSaving()) { ar << *getRawValuePtr<TYPE>(target); } else { TYPE value = 0; ar << value; setRawValuePtr(target, value); }
#define COPYTO(TYPE) setRawValuePtr(dest, getRawValuePtr<TYPE>(source))

IMPLEMENT_CLASS_NO_CTR(NumbericProperty)
IMPLEMENT_CLASS_NO_CTR(BoolProperty)

void BoolProperty::serializeElement(void *target, Archive &ar) {
    SERIALIZER(bool);
}

BoolProperty::BoolProperty(Struct *target, const String &name, uint64_t offset)
    : NumbericProperty(target, name, offset) {
    setSize(sizeof(bool));
}

void BoolProperty::copyTo(void *dest, void *source) {
    COPYTO(bool);
}

IMPLEMENT_CLASS_NO_CTR(IntProperty)

void IntProperty::serializeElement(void *target, Archive &ar) {
    SERIALIZER(int);
}

IntProperty::IntProperty(Struct *target, const String &name, uint64_t offset) : NumbericProperty(target, name, offset) {
    setSize(sizeof(int));
}

void IntProperty::copyTo(void *dest, void *source) {
    COPYTO(int);
}

IMPLEMENT_CLASS_NO_CTR(Int8Property)

void Int8Property::serializeElement(void *target, Archive &ar) {
    SERIALIZER(int8_t);
}

Int8Property::Int8Property(Struct *target, const String &name, uint64_t offset) : NumbericProperty(target, name, offset) {
    setSize(sizeof(int8_t));
}

void Int8Property::copyTo(void *dest, void *source) {
    COPYTO(int8_t);
}

IMPLEMENT_CLASS_NO_CTR(Int32Property)

void Int32Property::serializeElement(void *target, Archive &ar) {
    SERIALIZER(int32_t);
}

Int32Property::Int32Property(Struct *target, const String &name, uint64_t offset) : NumbericProperty(target, name, offset) {
    setSize(sizeof(int32_t));
}

void Int32Property::copyTo(void *dest, void *source) {
    COPYTO(int32_t);
}

IMPLEMENT_CLASS_NO_CTR(Int64Property)

void Int64Property::serializeElement(void *target, Archive &ar) {
    SERIALIZER(int64_t);
}

Int64Property::Int64Property(Struct *target, const String &name, uint64_t offset) : NumbericProperty(target, name, offset) {
    setSize(sizeof(int64_t));
}

void Int64Property::copyTo(void *dest, void *source) {
    COPYTO(int64_t);
}

IMPLEMENT_CLASS_NO_CTR(FloatProperty)

void FloatProperty::serializeElement(void *target, Archive &ar) {
    SERIALIZER(float);
}

FloatProperty::FloatProperty(Struct *target, const String &name, uint64_t offset) : NumbericProperty(target, name, offset) {
    setSize(sizeof(float));
}

void FloatProperty::copyTo(void *dest, void *source) {
    COPYTO(float);
}

IMPLEMENT_CLASS_NO_CTR(DoubleProperty)

void DoubleProperty::serializeElement(void *target, Archive &ar) {
    SERIALIZER(double);
}

DoubleProperty::DoubleProperty(Struct *target, const String &name, uint64_t offset) : NumbericProperty(target, name, offset) {
    setSize(sizeof(double));
}

void DoubleProperty::copyTo(void *dest, void *source) {
    COPYTO(double);
}

IMPLEMENT_CLASS_NO_CTR(ObjectProperty)

ObjectProperty::ObjectProperty(Struct *target, const String &name, uint64_t offset) : Property(nullptr, name, offset), mTarget(target) { }

Struct *ObjectProperty::getTarget() const {
    return mTarget;
}

IMPLEMENT_CLASS_NO_CTR(StructProperty)

StructProperty::StructProperty(Struct *target, const String &name, uint64_t offset) : ObjectProperty(target, name, offset) {
}

void StructProperty::serializeElement(void *target, Archive &ar) {
    if (ar.isSaving()) {
        auto fields = getTarget()->getCppProperties();

        size_t length = fields.length();
        ar << length;

        for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            ar << name;
            property->serializeElement(target, ar);
        }

    } else {
        size_t length = 0;
        ar << length;

        for (int i = 0; i < length; i++) {
            String name;
            ar << name;
            if (name.empty()) {
                break;
            }

            auto field = getTarget()->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            property->serializeElement(target, ar);
        }
    }
}

void StructProperty::copyTo(void *dest, void *source) {
    auto fields = getTarget()->getCppProperties();

    for (auto field : fields) {
        if (!field->isA<Property>()) {
            continue;
        }

        auto property = (Property *) field;
        String &name = const_cast<String &>(property->getName());

        ((Property *) field)->copyTo(dest, source);
    }
}

const size_t &StructProperty::getSize() {
    if (Property::getSize() == 0) {
        auto properties = getTarget()->getCppProperties();
        size_t size = 0;

        for (auto &property : properties) {
            if (property->isA<Property>()) {
                size += ((Property *) property)->getSize();
            } else if (property->isA<Struct>()) {
                size += ((Struct *) property)->getSize();
            } else {
                LOG(LogReflection, Warning, TEXT("Unable to find size from type '%s'"), *property->getName());
            }
        }

        Property::setSize(size);
    }

    return Property::getSize();
}



IMPLEMENT_CLASS_NO_CTR(ClassProperty)

ClassProperty::ClassProperty(Struct *target, const String &name, uint64_t offset) : ObjectProperty(target, name, offset) {
}

void ClassProperty::serializeElement(void *target, Archive &ar) {
    void **ptr = (void **) target;
    if (ar.isSaving()) {
        auto fields = getTarget()->getCppProperties();

        size_t length = fields.length();
        ar << length;

        for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            ar << name;
            property->serializeElement(*ptr, ar);
        }
    } else {
        size_t size = getTarget()->getSize();
        *ptr = malloc(size);

        size_t length = 0;
        ar << length;

        ((Class *) getTarget())->classConstructor(*ptr);

        for (int i = 0; i < length; i++) {
            String name;
            ar << name;
            if (name.empty()) {
                break;
            }

            auto field = getTarget()->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            property->serializeElement(*ptr, ar);
        }
    }
}

void ClassProperty::copyTo(void *dest, void *source) {
    auto fields = getTarget()->getCppProperties();

    for (auto field : fields) {
        if (!field->isA<Property>()) {
            continue;
        }

        auto property = (Property *) field;
        String &name = const_cast<String &>(property->getName());

        ((Property *) field)->copyTo(dest, source);
    }
}

const size_t &ClassProperty::getSize() {
    if (Property::getSize() == 0) {
        auto properties = getTarget()->getCppProperties();
        size_t size = 0;

        for (auto &property : properties) {
            if (property->isA<Property>()) {
                size += ((Property *) property)->getSize();
            } else if (property->isA<Struct>()) {
                size += ((Struct *) property)->getSize();
            } else {
                LOG(LogReflection, Warning, TEXT("Unable to find size from type '%s'"), *property->getName());
            }
        }

        Property::setSize(size);
    }

    return Property::getSize();
}

IMPLEMENT_CLASS_NO_CTR(ArrayProperty)

void ArrayProperty::serializeElement(void *target, Archive &ar) {
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

ArrayProperty::ArrayProperty(Struct *target, const String &name, uint64_t offset) : Property(target, name, offset) {
    setSize(sizeof(TArray<uint8_t>));
}

void ArrayProperty::copyTo(void *dest, void *source) {
    Property::copyTo(dest, source);
}

IMPLEMENT_CLASS_NO_CTR(MapProperty)

MapProperty::MapProperty(Struct *target, const String &name, uint64_t offset) : Property(target, name, offset) {
    setSize(sizeof(TMap<uint8_t, uint8_t>));
}

void MapProperty::serializeElement(void *target, Archive &ar) {
    auto ptr = (uint8_t *) target;
    if (ar.isSaving()) {
        // get data as list of pair
        TArray<uint8_t> &array = *getRawValuePtr<TArray<uint8_t>>(target);

        ptr += (::size_t) &reinterpret_cast<char const volatile &>((((TMap<uint8_t, uint8_t> *) nullptr)->mCapacity));
        size_t capacity = *(size_t *) ptr;

        ptr += sizeof(size_t);
        size_t length = *(size_t *) ptr;

        auto *data = array.getData();

        ar << capacity;
        ar << length;
        for (auto index = 0; index < length;) {
            mKeyType->serializeElement(data, ar);
            data += mKeyType->getSize();
            index += mKeyType->getSize();

            mValueType->serializeElement(data, ar);
            data += mValueType->getSize();
            index += mValueType->getSize();
        }
    } else {
        size_t capacity = 0;
        ar << capacity;

        size_t length = 0;
        ar << length;

        TArray<uint8_t> &array = *getRawValuePtr<TArray<uint8_t>>(target);
        array.reserve(capacity);
        array.resize(length);
        auto *data = array.getData();

        for (auto index = 0; index < length;) {
            mKeyType->serializeElement(data, ar);
            data += mKeyType->getSize();
            index += mKeyType->getSize();

            mValueType->serializeElement(data, ar);
            data += mValueType->getSize();
            index += mValueType->getSize();
        }
    }
}

void MapProperty::copyTo(void *dest, void *source) {
    Property::copyTo(dest, source);
}

IMPLEMENT_CLASS_NO_CTR(StringProperty)

void StringProperty::serializeElement(void *target, Archive &ar) {
    if (ar.isSaving()) {
        ar << *getRawValuePtr<String>(target);
    } else {
        new (((uint8_t *) target) + getOffset()) String();

        String value = TEXT("");
        ar << value;
        setRawValuePtr(target, value);
    }
}

StringProperty::StringProperty(Struct *target, const String &name, uint64_t offset) : Property(target, name, offset) {
    setSize(sizeof(String));
}

void StringProperty::copyTo(void *dest, void *source) {
    COPYTO(String);
}
