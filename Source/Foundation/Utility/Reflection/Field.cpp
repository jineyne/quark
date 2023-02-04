#include "Field.h"

#include "Reflection/Class.h"
#include "Reflection/Reflection.h"

IMPLEMENT_CLASS_NO_CTR(QField)

QField::QField(QClass *myClass, const FString &name) : QObject(myClass, name) {}

FString QField::getMetaData(const FString &key) {
    return mMetaData.getValue(key);
}

void QField::setMetaData(const FString &key, const FString &value) {
    mMetaData.setValue(key, value);
}

bool QField::hasMetaData(const FString &key) {
    return mMetaData.contains(key);
}