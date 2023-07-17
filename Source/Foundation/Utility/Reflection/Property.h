#pragma once

#include <cstddef>
#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Field.h"
#include "Reflection/Class.h"

class DLL_EXPORT Property : public Field {
private:
    int32_t mElementSize = 0;
    int32_t mArraySize = 1;
    size_t mSize = 0;
    size_t mOffset = 0;

    Struct *mOwner = nullptr;

public:
    Property(Struct *owner, const String &name, uint64_t flags, size_t offset);

public:
    template <typename T>
    T *getValuePtr(Object *object, int32_t arrayIndex = 0) {
        assert(arrayIndex < mArraySize);
        assert(object);

        return (T *) (((uint8_t *) object) + mOffset + (mElementSize * arrayIndex));
    }

    template <typename T>
    void setValuePtr(Object *object, T value, int32_t arrayIndex = 0) {
        assert(arrayIndex < mArraySize);
        assert(object);

        * (T *) ((((uint8_t *) object) + mOffset + (mElementSize * arrayIndex))) = std::move(value);
    }

    template <typename T>
    T *getRawValuePtr(void *object, int32_t arrayIndex = 0) {
        return (T *) (((uint8_t *) object) + mOffset + (mElementSize * arrayIndex));
    }

    template <typename T>
    void setRawValuePtr(void *object, T value, int32_t arrayIndex = 0) {
        * (T *) ((((uint8_t *) object) + mOffset + (mElementSize * arrayIndex))) = std::move(value);
    }

    virtual void copyTo(void *dest, void* source) { }

    virtual const int32_t &getArraySize() const { return mArraySize; }
    void setArraySize(const int32_t &size) { mArraySize = size; }

    virtual const size_t &getSize() { return mSize; }
    const size_t &getOffset() const { return mOffset; }

    Struct *getOwner() const { return mOwner; }

    virtual void serializeElement(void *target, ArchiveFormatter &af) {}

protected:
    void setSize(size_t size) { mSize = size; }

public:
    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Property, Field, NO_API);

    friend class Reflection;
};

class DLL_EXPORT NumericProperty : public Property {
public:
    NumericProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset) : Property(target, name, flags, offset) {}

public:

    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(NumericProperty , Property, NO_API);
};

class DLL_EXPORT BoolProperty : public NumericProperty {
public:
    BoolProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(BoolProperty , NumericProperty, NO_API);
};

class DLL_EXPORT IntProperty : public NumericProperty {
public:
    IntProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(IntProperty, NumericProperty, NO_API);
};

class DLL_EXPORT Int8Property : public NumericProperty {
public:
    Int8Property(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Int8Property, NumericProperty, NO_API);
};

class DLL_EXPORT Int32Property : public NumericProperty {
public:
    Int32Property(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Int32Property, NumericProperty, NO_API);
};

class DLL_EXPORT Int64Property : public NumericProperty {
public:
    Int64Property(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Int64Property, NumericProperty, NO_API);
};

class DLL_EXPORT FloatProperty : public NumericProperty {
public:
    FloatProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(FloatProperty, NumericProperty, NO_API);
};

class DLL_EXPORT DoubleProperty : public NumericProperty {
public:
    DoubleProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(DoubleProperty, NumericProperty, NO_API);
};

class DLL_EXPORT ObjectProperty : public Property {
private:
    Struct *mTarget = nullptr;

public:
    ObjectProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    Struct *getTarget() const;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(ObjectProperty, Property, NO_API);
};

class DLL_EXPORT StructProperty : public ObjectProperty {
public:
    StructProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;
    void copyTo(void *dest, void *source) override;

    const size_t &getSize() override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(StructProperty, ObjectProperty, NO_API);
};

class DLL_EXPORT ClassProperty : public StructProperty {
public:
    ClassProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(ClassProperty, StructProperty, NO_API);
};

class DLL_EXPORT ResourceProperty : public ObjectProperty {
private:
    ObjectProperty *mResourceType;

public:
    ResourceProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;
    void copyTo(void *dest, void *source) override;

    ObjectProperty *getResourceType() const;
    void setResourceType(ObjectProperty *resourceType);

    const size_t &getSize() override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(ResourceProperty, ObjectProperty, NO_API);
};

class DLL_EXPORT ArrayProperty : public Property {
private:
    Property *mTemplateType = nullptr;

public:
    ArrayProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

    void setTemplateType(Property *type) { mTemplateType = type; }
    Property *getTemplateType() const { return mTemplateType; }

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(ArrayProperty, Property, NO_API);
};

class DLL_EXPORT MapProperty : public Property {
private:
    Property *mKeyType = nullptr;
    Property *mValueType = nullptr;

    TFunction<void(uint8_t *, uint8_t *, uint8_t *)> mFnAdd;

public:
    MapProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

    void setKeyType(Property *type) { mKeyType = type; }
    Property *getKeyType() const { return mKeyType; }

    void setValueType(Property *type) { mValueType = type; }
    Property *getValueType() const { return mValueType; }

    TFunction<void(uint8_t *, uint8_t *, uint8_t *)> getAddFunction() const { return mFnAdd; }
    void setAddFunction(TFunction<void(uint8_t *, uint8_t *, uint8_t *)> fn) { mFnAdd = fn; }

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(MapProperty, Property, NO_API);
};

class DLL_EXPORT StringProperty : public Property {
public:
    StringProperty(Struct *target, const String &name, uint64_t flags, uint64_t offset);

public:
    void serializeElement(void *target, ArchiveFormatter &formatter) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(StringProperty, Property, NO_API);
};
