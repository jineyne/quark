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

IMPLEMENT_CLASS(QNumbericProperty)
IMPLEMENT_CLASS(QBoolProperty)
IMPLEMENT_CLASS(QIntProperty)
IMPLEMENT_CLASS(QInt8Property)
IMPLEMENT_CLASS(QInt32Property)
IMPLEMENT_CLASS(QInt64Property)
IMPLEMENT_CLASS(QFloatProperty)
IMPLEMENT_CLASS(QDoubleProperty)

IMPLEMENT_CLASS(QObjectProperty)
IMPLEMENT_CLASS(QStructProperty)
IMPLEMENT_CLASS(QClassProperty)

IMPLEMENT_CLASS(QArrayProperty)