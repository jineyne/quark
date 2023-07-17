#include "Property.h"

#include "Memory/FixedSizeElementAllocator.h"
#include "Reflection/Class.h"
#include "Reflection/Reflection.h"
#include "ObjectHash.h"

IMPLEMENT_CLASS_NO_CTR(Property)

Property::Property(Struct *owner, const String &name, uint64_t flags, size_t offset)
        : Field(Property::StaticClass(), name, flags), mOwner(owner), mOffset(offset) {
    if (owner) {
        owner->addCppProperty(this);
    }
}

#define SERIALIZER(TYPE) \
    if (formatter.isSaving()) { \
        TYPE value = *getRawValuePtr<TYPE>(target); \
        formatter.serialize(value); \
    } else { \
        TYPE value = 0;  \
        formatter.serialize(value);                 \
        setRawValuePtr(target, value);              \
    }
#define COPYTO(TYPE) setRawValuePtr(dest, getRawValuePtr<TYPE>(source))

IMPLEMENT_CLASS_NO_CTR(NumericProperty)
IMPLEMENT_CLASS_NO_CTR(BoolProperty)

void BoolProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    SERIALIZER(bool);

}

BoolProperty::BoolProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : NumericProperty(target, name, flags, offset) {
    setSize(sizeof(bool));
}

void BoolProperty::copyTo(void *dest, void *source) {
    COPYTO(bool);
}

IMPLEMENT_CLASS_NO_CTR(IntProperty)

void IntProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    SERIALIZER(int);
}

IntProperty::IntProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : NumericProperty(target, name, flags, offset) {
    setSize(sizeof(int));
}

void IntProperty::copyTo(void *dest, void *source) {
    COPYTO(int);
}

IMPLEMENT_CLASS_NO_CTR(Int8Property)

void Int8Property::serializeElement(void *target, ArchiveFormatter &formatter) {
    SERIALIZER(int8_t);
}

Int8Property::Int8Property(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : NumericProperty(target, name, flags, offset) {
    setSize(sizeof(int8_t));
}

void Int8Property::copyTo(void *dest, void *source) {
    COPYTO(int8_t);
}

IMPLEMENT_CLASS_NO_CTR(Int32Property)

void Int32Property::serializeElement(void *target, ArchiveFormatter &formatter) {
    SERIALIZER(int32_t);
}

Int32Property::Int32Property(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : NumericProperty(target, name, flags, offset) {
    setSize(sizeof(int32_t));
}

void Int32Property::copyTo(void *dest, void *source) {
    COPYTO(int32_t);
}

IMPLEMENT_CLASS_NO_CTR(Int64Property)

void Int64Property::serializeElement(void *target, ArchiveFormatter &formatter) {
    SERIALIZER(int64_t);
}

Int64Property::Int64Property(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : NumericProperty(target, name, flags, offset) {
    setSize(sizeof(int64_t));
}

void Int64Property::copyTo(void *dest, void *source) {
    COPYTO(int64_t);
}

IMPLEMENT_CLASS_NO_CTR(FloatProperty)

void FloatProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    SERIALIZER(float);
}

FloatProperty::FloatProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : NumericProperty(target, name, flags, offset) {
    setSize(sizeof(float));
}

void FloatProperty::copyTo(void *dest, void *source) {
    COPYTO(float);
}

IMPLEMENT_CLASS_NO_CTR(DoubleProperty)

void DoubleProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    SERIALIZER(double);
}

DoubleProperty::DoubleProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : NumericProperty(target, name, flags, offset) {
    setSize(sizeof(double));
}

void DoubleProperty::copyTo(void *dest, void *source) {
    COPYTO(double);
}

IMPLEMENT_CLASS_NO_CTR(ObjectProperty)

ObjectProperty::ObjectProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : Property(nullptr, name, flags, offset), mTarget(target) { }

Struct *ObjectProperty::getTarget() const {
    return mTarget;
}

IMPLEMENT_CLASS_NO_CTR(StructProperty)

StructProperty::StructProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : ObjectProperty(target, name, flags, offset) { }

void StructProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    Struct *clazz = getTarget();

    formatter.enterRecord();

    if (formatter.isSaving()) {
        auto fields = clazz->getCppProperties();

        for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            formatter.enterField(name);
            property->serializeElement(target, formatter);
            formatter.leaveField();
        }

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
            size_t size = clazz->getSize();
            * (void **) target = q_alloc(size);
            ((Class *) getTarget())->classConstructor(* (void **) target);
        }

        String fieldName = String::Empty;
        while ((formatter.enterField(fieldName), !fieldName.empty())) {
            auto field = clazz->getCppPropertiesByName(fieldName);
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

ClassProperty::ClassProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : StructProperty(target, name, flags, offset) { }

void ClassProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    Super::serializeElement(target, formatter);

    // object 및 structure와의 다른점은 포인터에 메모리를 할당한다는 것
    /*auto clazz = getTarget();

    formatter.enterRecord();

    void **ptr = (void **) target;
    if (formatter.isSaving()) {
        auto fields = clazz->getCppProperties();

        for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            formatter.enterField(name);
            property->serializeElement(*ptr, formatter);
            formatter.leaveField();
        }
    } else {
        size_t size = clazz->getSize();
        *ptr = q_alloc(size);
        ((Class *) getTarget())->classConstructor(*ptr);

        String fieldName = String::Empty;
        while ((formatter.enterField(fieldName), !fieldName.empty())) {
            auto field = clazz->getCppPropertiesByName(fieldName);
            if (!field) {
                formatter.leaveField();
                break;
            }

            if (!field->isA<Property>()) {
                formatter.leaveField();
                break;
            }

            auto property = (Property *) field;
            property->serializeElement(*ptr, formatter);
            formatter.leaveField();
        }
    }

    formatter.leaveRecord();*/
}

IMPLEMENT_CLASS_NO_CTR(ArrayProperty)

void ArrayProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    /**
     * 실제 배열 길이와 데이터 길이를 고려해서 archive에 저장되는건 실제 요소로 변경하고
     * 데이터 길이는 프로그램 상에서 처리한다.
     *
     * TODO: 근데 만약 클래스 크기가 바뀌었다면?
     */

    static String lengthName = TEXT("length");

    if (formatter.isSaving()) {
        TArray<uint8_t> &array = *getRawValuePtr<TArray<uint8_t>>(target);
        int32_t length = array.length();
        uint8_t *data = array.getData();

        int32_t elementCounts = length / mTemplateType->getSize();
        formatter.enterArray(elementCounts);

        formatter.enterAttribute(lengthName);
        formatter.serialize(length);
        formatter.leaveAttribute();

        for (auto index = 0; index < length;) {
            formatter.enterArrayElement();

            mTemplateType->serializeElement(data, formatter);
            data += mTemplateType->getSize();
            index += mTemplateType->getSize();

            formatter.leaveArrayElement();
        }

        formatter.leaveArray();
    } else {
        int32_t elementCounts = 0;
        formatter.enterArray(elementCounts);

        // ignore length attribute
        int32_t length = 0;
        formatter.enterAttribute(lengthName);
        formatter.serialize(length);
        formatter.leaveAttribute();

        TArray<uint8_t> &array = *getRawValuePtr<TArray<uint8_t>>(target);
        array.resize(length);
        uint8_t *data = array.getData();

        size_t debug = 0;
        for (auto index = 0; index < length;) {
            formatter.enterArrayElement();

            mTemplateType->serializeElement(data, formatter);

            data += mTemplateType->getSize();
            index += mTemplateType->getSize();

            formatter.leaveArrayElement();
            debug++;
        }

        formatter.leaveArray();
        assert(debug == elementCounts);
    }
}

IMPLEMENT_CLASS_NO_CTR(ResourceProperty)

ResourceProperty::ResourceProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
        : ObjectProperty(target, name, flags, offset) { }

void ResourceProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    Property::serializeElement(target, formatter);
}

void ResourceProperty::copyTo(void *dest, void *source) {
    Property::copyTo(dest, source);
}

ObjectProperty *ResourceProperty::getResourceType() const {
    return mResourceType;
}

void ResourceProperty::setResourceType(ObjectProperty *resourceType) {
    mResourceType = resourceType;
}

const size_t &ResourceProperty::getSize() {
    return Property::getSize();
}

ArrayProperty::ArrayProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : Property(target, name, flags, offset) {
    setSize(sizeof(TArray<uint8_t>));
}

void ArrayProperty::copyTo(void *dest, void *source) {
    Property::copyTo(dest, source);
}

IMPLEMENT_CLASS_NO_CTR(MapProperty)

MapProperty::MapProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : Property(target, name, flags, offset) {
    setSize(sizeof(TMap<uint8_t, uint8_t>));
}

void MapProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    static String capacityName = TEXT("capacity");
    static String lengthName = TEXT("length");
    static String indexName = TEXT("index");
    static String keyName = TEXT("key");
    static String valueName = TEXT("value");

    TArray<uint8_t> &array = *getRawValuePtr<TArray<uint8_t>>(target);
    uint8_t *ptr = (uint8_t *) &array;

    if (formatter.isSaving()) {
        ptr += (::size_t) &reinterpret_cast<char const volatile &>((((TMap<uint8_t, uint8_t> *) nullptr)->mCapacity));
        size_t capacity = *(size_t *) ptr;

        ptr += sizeof(size_t);
        size_t length = *(size_t *) ptr;

        int32_t elementCounts = length;
        formatter.enterArray(elementCounts);

        formatter.enterAttribute(capacityName);
        formatter.serialize(capacity);
        formatter.leaveAttribute();

        formatter.enterAttribute(lengthName);
        formatter.serialize(length);
        formatter.leaveAttribute();

        auto *buckets = array.getData();

        for (int32_t bucketIndex = 0; bucketIndex < capacity; bucketIndex++) {
            auto &bucket = *(std::list<uint8_t>*) buckets;

            if (bucket.size() != 0) {
                auto data = &(*bucket.begin());
                int32_t bucketLength = bucket.size();

                if (bucketLength == 0) {
                    continue;
                }

                formatter.enterArrayElement();
                formatter.enterArray(bucketLength);

                formatter.enterAttribute(indexName);
                formatter.serialize(bucketIndex);
                formatter.leaveAttribute();

                for (auto elementIndex = 0; elementIndex < bucketLength; elementIndex++) {
                    formatter.enterArrayElement();

                    formatter.enterField(keyName);

                    mKeyType->serializeElement(data, formatter);
                    data += mKeyType->getSize();

                    formatter.leaveField();

                    formatter.enterField(valueName);

                    mValueType->serializeElement(data, formatter);
                    data += mValueType->getSize();

                    formatter.leaveField();

                    formatter.leaveArrayElement();
                }

                formatter.leaveArray();
                formatter.leaveArrayElement();
            }

            buckets += sizeof(std::list<uint8_t>);
        }
    } else {
        int32_t elementCounts = 0;
        formatter.enterMap(elementCounts);

        size_t capacity = 0;
        size_t length = 0;

        formatter.enterAttribute(capacityName);
        formatter.serialize(capacity);
        formatter.leaveAttribute();

        formatter.enterAttribute(lengthName);
        formatter.serialize(length);
        formatter.leaveAttribute();

        // array.resize(capacity);

        uint8_t *keyPtr = q_alloc<uint8_t>(mKeyType->getSize());
        uint8_t *valuePtr = q_alloc<uint8_t>(mValueType->getSize());

        using MapType = TMap<uint8_t *, uint8_t *, std::hash<uint8_t *>, std::equal_to<uint8_t *>, StdFixedSizeElementAllocator<TPair<uint8_t *, uint8_t *>>>;

        MapType *data = * (MapType **) &ptr;
        MapType::AllocatorType::Internal.init(mKeyType->getSize() * mValueType->getSize());
        StdFixedSizeElementAllocator<std::_List_node<TPair<uint8_t *, uint8_t *>, void*>>::Internal.init(mKeyType->getSize() * mValueType->getSize());

        data->mTable.reserve(capacity);
        data->mLength = length;
        data->mCapacity = capacity;

        size_t debug = 0;
        for (auto index = 0; index < length; index++) {
            formatter.enterArrayElement();

            int32_t bucketLength = 0;
            formatter.enterArray(bucketLength);

            int32_t bucketIndex;
            formatter.enterAttribute(indexName);
            formatter.serialize(bucketIndex);
            formatter.leaveAttribute();

            // move index to bucket index
            while (index < bucketIndex) {
                index++;
            }

            for (auto elementIndex = 0; elementIndex < bucketLength; elementIndex++) {
                formatter.enterArrayElement();
                formatter.enterField(keyName);

                mKeyType->serializeElement(keyPtr, formatter);

                formatter.leaveField();

                formatter.enterField(valueName);

                mValueType->serializeElement(valuePtr, formatter);

                formatter.leaveField();

                formatter.leaveArrayElement();

                TPair<uint8_t *, uint8_t *> pair = { nullptr, nullptr };
                data->mTable[index].push_back(pair);

                auto pairPtr = (uint8_t *) &data->mTable[index].back().key;
                memcpy(pairPtr, keyPtr, mKeyType->getSize());
                pairPtr += mKeyType->getSize();
                memcpy(pairPtr, valuePtr, mValueType->getSize());

                debug++;
            }

            formatter.leaveArrayElement();
        }

        q_free(keyPtr);
        q_free(valuePtr);

        assert(elementCounts == debug);
    }

    formatter.leaveArray();
}

void MapProperty::copyTo(void *dest, void *source) {
    Property::copyTo(dest, source);
}

IMPLEMENT_CLASS_NO_CTR(StringProperty)

void StringProperty::serializeElement(void *target, ArchiveFormatter &formatter) {
    if (formatter.isSaving()) {
        formatter.serialize(*getRawValuePtr<String>(target));
    } else {
        new (((uint8_t *) target) + getOffset()) String();

        String value = TEXT("");
        formatter.serialize(value);
        setRawValuePtr(target, value);
    }
}

StringProperty::StringProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset)
    : Property(target, name, flags, offset) {
    setSize(sizeof(String));
}

void StringProperty::copyTo(void *dest, void *source) {
    COPYTO(String);
}
