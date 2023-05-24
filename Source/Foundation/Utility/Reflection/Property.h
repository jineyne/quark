#pragma once

#include <cstddef>
#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Field.h"
#include "Reflection/Class.h"

class DLL_EXPORT QProperty : public QField {
private:
    int32_t mElementSize = 0;
    int32_t mArraySize = 1;
    size_t mSize = 0;
    size_t mOffset = 0;

    QStruct *mOwner = nullptr;

public:
    QProperty(QStruct *owner, const FString &name, size_t offset);

public:
    template <typename T>
    T *getValuePtr(QObject *object, int32_t arrayIndex = 0) {
        assert(arrayIndex < mArraySize);
        assert(object);

        return (T *) (((uint8_t *) object) + mOffset + (mElementSize * arrayIndex));
    }

    template <typename T>
    void setValuePtr(QObject *object, T value, int32_t arrayIndex = 0) {
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

    QStruct *getOwner() const { return mOwner; }

    virtual void serializeElement(void *target, FArchive &ar) {}

protected:
    void setSize(size_t size) { mSize = size; }

public:
    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QProperty, QField, NO_API);

    friend class QReflection;
};

class DLL_EXPORT QNumbericProperty : public QProperty {
public:
    QNumbericProperty(QStruct *target, const FString &name, uint64_t offset) : QProperty(target, name, offset) {}

public:

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QNumbericProperty , QProperty, NO_API);
};

class DLL_EXPORT QBoolProperty : public QNumbericProperty {
public:
    QBoolProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QBoolProperty , QNumbericProperty, NO_API);
};

class DLL_EXPORT QIntProperty : public QNumbericProperty {
public:
    QIntProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QIntProperty, QNumbericProperty, NO_API);
};

class DLL_EXPORT QInt8Property : public QNumbericProperty {
public:
    QInt8Property(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QInt8Property, QNumbericProperty, NO_API);
};

class DLL_EXPORT QInt32Property : public QNumbericProperty {
public:
    QInt32Property(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QInt32Property, QNumbericProperty, NO_API);
};

class DLL_EXPORT QInt64Property : public QNumbericProperty {
public:
    QInt64Property(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QInt64Property, QNumbericProperty, NO_API);
};

class DLL_EXPORT QFloatProperty : public QNumbericProperty {
public:
    QFloatProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QFloatProperty, QNumbericProperty, NO_API);
};

class DLL_EXPORT QDoubleProperty : public QNumbericProperty {
public:
    QDoubleProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QDoubleProperty, QNumbericProperty, NO_API);
};

class DLL_EXPORT QObjectProperty : public QProperty {
private:
    QStruct *mTarget = nullptr;

public:
    QObjectProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    QStruct *getTarget() const;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QObjectProperty, QProperty, NO_API);
};

class DLL_EXPORT QStructProperty : public QObjectProperty {
public:
    QStructProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;
    void copyTo(void *dest, void *source) override;

    const size_t &getSize() override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QStructProperty, QObjectProperty, NO_API);
};

class DLL_EXPORT QClassProperty : public QObjectProperty {
public:
    QClassProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;
    void copyTo(void *dest, void *source) override;

    const size_t &getSize() override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QClassProperty, QObjectProperty, NO_API);
};

class DLL_EXPORT QArrayProperty : public QProperty {
private:
    QProperty *mTemplateType = nullptr;

public:
    QArrayProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

    void setTemplateType(QProperty *type) { mTemplateType = type; }
    QProperty *getTemplateType() const { return mTemplateType; }

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QArrayProperty, QProperty, NO_API);
};

class DLL_EXPORT QMapProperty : public QProperty {
private:
    QProperty *mKeyType = nullptr;
    QProperty *mValueType = nullptr;

public:
    QMapProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

    void setKeyType(QProperty *type) { mKeyType = type; }
    QProperty *getKeyType() const { return mKeyType; }

    void setValueType(QProperty *type) { mValueType = type; }
    QProperty *getValueType() const { return mValueType; }

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QMapProperty, QProperty, NO_API);
};

class DLL_EXPORT QStringProperty : public QProperty {
public:
    QStringProperty(QStruct *target, const FString &name, uint64_t offset);

public:
    void serializeElement(void *target, FArchive &ar) override;

    void copyTo(void *dest, void *source) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QStringProperty, QProperty, NO_API);
};
