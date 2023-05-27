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
    Property(Struct *owner, const String &name, size_t offset);

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

    virtual void serializeElement(void *target, Archive &ar) {}

protected:
    void setSize(size_t size) { mSize = size; }

public:
    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Property, Field, NO_API);

    friend class Reflection;
};

class DLL_EXPORT NumbericProperty : public Property {
public:
    NumbericProperty(Struct *target, const String &name, uint64_t offset) : Property(target, name, offset) {}

public:

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(NumbericProperty , Property, NO_API);
};

class DLL_EXPORT BoolProperty : public NumbericProperty {
public:
    BoolProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(BoolProperty , NumbericProperty, NO_API);
};

class DLL_EXPORT IntProperty : public NumbericProperty {
public:
    IntProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(IntProperty, NumbericProperty, NO_API);
};

class DLL_EXPORT Int8Property : public NumbericProperty {
public:
    Int8Property(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Int8Property, NumbericProperty, NO_API);
};

class DLL_EXPORT Int32Property : public NumbericProperty {
public:
    Int32Property(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Int32Property, NumbericProperty, NO_API);
};

class DLL_EXPORT Int64Property : public NumbericProperty {
public:
    Int64Property(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(Int64Property, NumbericProperty, NO_API);
};

class DLL_EXPORT FloatProperty : public NumbericProperty {
public:
    FloatProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(FloatProperty, NumbericProperty, NO_API);
};

class DLL_EXPORT DoubleProperty : public NumbericProperty {
public:
    DoubleProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(DoubleProperty, NumbericProperty, NO_API);
};

class DLL_EXPORT ObjectProperty : public Property {
private:
    Struct *mTarget = nullptr;

public:
    ObjectProperty(Struct *target, const String &name, uint64_t offset);

public:
    Struct *getTarget() const;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(ObjectProperty, Property, NO_API);
};

class DLL_EXPORT StructProperty : public ObjectProperty {
public:
    StructProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;
    void copyTo(void *dest, void *source) override;

    const size_t &getSize() override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(StructProperty, ObjectProperty, NO_API);
};

class DLL_EXPORT ClassProperty : public ObjectProperty {
public:
    ClassProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;
    void copyTo(void *dest, void *source) override;

    const size_t &getSize() override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(ClassProperty, ObjectProperty, NO_API);
};

class DLL_EXPORT ArrayProperty : public Property {
private:
    Property *mTemplateType = nullptr;

public:
    ArrayProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

    void setTemplateType(Property *type) { mTemplateType = type; }
    Property *getTemplateType() const { return mTemplateType; }

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(ArrayProperty, Property, NO_API);
};

class DLL_EXPORT MapProperty : public Property {
private:
    Property *mKeyType = nullptr;
    Property *mValueType = nullptr;

public:
    MapProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

    void setKeyType(Property *type) { mKeyType = type; }
    Property *getKeyType() const { return mKeyType; }

    void setValueType(Property *type) { mValueType = type; }
    Property *getValueType() const { return mValueType; }

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(MapProperty, Property, NO_API);
};

class DLL_EXPORT StringProperty : public Property {
public:
    StringProperty(Struct *target, const String &name, uint64_t offset);

public:
    void serializeElement(void *target, Archive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(StringProperty, Property, NO_API);
};
