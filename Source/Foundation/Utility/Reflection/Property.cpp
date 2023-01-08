#include "Property.h"

#include "Reflection/Class.h"
#include "Reflection/Reflection.h"

IMPLEMENT_CLASS(QProperty)

QProperty::QProperty(QStruct *target, const FString &name, uint64_t offset)
        : QField(QProperty::StaticClass(), name), mOffset(offset) {
    if (target) {
        target->addCppProperty(this);
    }
}

#define SERIALIZER(TYPE) if (ar.isSaving()) { ar << *getValuePtr<TYPE>(target); } else { TYPE value = 0; ar << value; setValuePtr(target, value); }

IMPLEMENT_CLASS(QNumbericProperty)
IMPLEMENT_CLASS(QBoolProperty)

void QBoolProperty::serializer(QObject *target, FArchive &ar) {
    SERIALIZER(bool);
}

IMPLEMENT_CLASS(QIntProperty)

void QIntProperty::serializer(QObject *target, FArchive &ar) {
    SERIALIZER(int);
}

IMPLEMENT_CLASS(QInt8Property)

void QInt8Property::serializer(QObject *target, FArchive &ar) {
    SERIALIZER(int8_t);
}

IMPLEMENT_CLASS(QInt32Property)

void QInt32Property::serializer(QObject *target, FArchive &ar) {
    SERIALIZER(int32_t);
}

IMPLEMENT_CLASS(QInt64Property)

void QInt64Property::serializer(QObject *target, FArchive &ar) {
    SERIALIZER(int64_t);
}

IMPLEMENT_CLASS(QFloatProperty)

void QFloatProperty::serializer(QObject *target, FArchive &ar) {
    SERIALIZER(float);
}

IMPLEMENT_CLASS(QDoubleProperty)

void QDoubleProperty::serializer(QObject *target, FArchive &ar) {
    SERIALIZER(double);
}

IMPLEMENT_CLASS(QObjectProperty)
IMPLEMENT_CLASS(QStructProperty)
IMPLEMENT_CLASS(QClassProperty)

IMPLEMENT_CLASS(QArrayProperty)

IMPLEMENT_CLASS(QStringProperty)

void QStringProperty::serializer(QObject *target, FArchive &ar) {
    if (ar.isSaving()) {
        ar << *getValuePtr<FString>(target);
    } else {
        FString value = TEXT("");
        ar << value;
        setValuePtr(target, value);
    }
}