#include "Field.h"

#include "Reflection/Class.h"
#include "Reflection/Reflection.h"

IMPLEMENT_CLASS_NO_CTR(Field)

Field::Field(Class *myClass, const String &name, uint64_t flags) : Object(myClass, name), mFlags(flags) {}

String Field::getMetaData(const String &key) {
    return mMetaData.getValue(key);
}

void Field::setMetaData(const String &key, const String &value) {
    mMetaData.setValue(key, value);
}

bool Field::hasMetaData(const String &key) {
    return mMetaData.contains(key);
}