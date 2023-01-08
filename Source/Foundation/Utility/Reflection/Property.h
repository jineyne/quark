#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Field.h"
#include "Reflection/Class.h"

class DLL_EXPORT QProperty : public QField {
private:
    int32_t mElementSize = 0;
    int32_t mArraySize = 1;
    size_t mSize;
    int32_t mOffset;

public:
    QProperty(QStruct *target, const FString &name, uint64_t offset);

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

        * (T *) ((((uint8_t *) object) + mOffset + (mElementSize * arrayIndex))) = value;
    }

    int32_t getArraySize() const { return mArraySize; }
    size_t getSize() const { return mSize; }

    virtual void serializer(QObject *target, FArchive &ar) {}

private:
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
    QBoolProperty(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QBoolProperty , QNumbericProperty, NO_API);
};

class DLL_EXPORT QIntProperty : public QNumbericProperty {
public:
    QIntProperty(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QIntProperty, QNumbericProperty, NO_API);
};

class DLL_EXPORT QInt8Property : public QNumbericProperty {
public:
    QInt8Property(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QInt8Property, QNumbericProperty, NO_API);
};

class DLL_EXPORT QInt32Property : public QNumbericProperty {
public:
    QInt32Property(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QInt32Property, QNumbericProperty, NO_API);
};

class DLL_EXPORT QInt64Property : public QNumbericProperty {
public:
    QInt64Property(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QInt64Property, QNumbericProperty, NO_API);
};

class DLL_EXPORT QFloatProperty : public QNumbericProperty {
public:
    QFloatProperty(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QFloatProperty, QNumbericProperty, NO_API);
};

class DLL_EXPORT QDoubleProperty : public QNumbericProperty {
public:
    QDoubleProperty(QStruct *target, const FString &name, uint64_t offset) : QNumbericProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QDoubleProperty, QNumbericProperty, NO_API);
};

class DLL_EXPORT QObjectProperty : public QProperty {
public:
    QObjectProperty(QStruct *target, const FString &name, uint64_t offset) : QProperty(target, name, offset) {}

public:
    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QObjectProperty, QProperty, NO_API);
};

class DLL_EXPORT QStructProperty : public QObjectProperty {
public:
    QStructProperty(QStruct *target, const FString &name, uint64_t offset) : QObjectProperty(target, name, offset) {}

public:
    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QStructProperty, QObjectProperty, NO_API);
};

class DLL_EXPORT QClassProperty : public QObjectProperty {
public:
    QClassProperty(QStruct *target, const FString &name, uint64_t offset) : QObjectProperty(target, name, offset) {}

public:
    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QClassProperty, QObjectProperty, NO_API);
};

class DLL_EXPORT QArrayProperty : public QProperty {
public:
    QArrayProperty(QStruct *target, const FString &name, uint64_t offset) : QProperty(target, name, offset) {}

public:
    DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QArrayProperty, QProperty, NO_API);
};

class DLL_EXPORT QStringProperty : public QProperty {
public:
    QStringProperty(QStruct *target, const FString &name, uint64_t offset) : QProperty(target, name, offset) {}

public:
    void serializer(QObject *target, FArchive &ar) override;

DECLARE_CASTED_CLASS_INTRINSIC_WITH_API(QStringProperty, QProperty, NO_API);
};
