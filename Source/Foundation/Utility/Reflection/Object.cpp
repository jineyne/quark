#include "Object.h"

#include "Reflection/Class.h"
#include "Reflection/Enum.h"
#include "Reflection/Reflection.h"
#include "Reflection/ObjectHash.h"

QClass *QObject::GetPrivateStaticClass() {
    static QClass *instance = nullptr;
    if (!instance) {
        // CREATE OBJECT MANUALLY?
        instance = new QClass(sizeof(QObject));
    }
    return instance;
}

QObject::QObject(QClass *myClass, const FString &name)
        : mClass(myClass), mName(name) { }

void QObject::serialize(FArchive &archive) {

}

void QObject::rename(const FString &name) {
    gObjectHash().remove(this);

    mName = name;

    gObjectHash().add(this);
}

void QObject::setClass(QClass* newClass) {
    gObjectHash().remove(this);

    mClass = newClass;

    gObjectHash().add(this);
}

void initClassOnStart(QClass *(*fnRegister)(), QClass *(*fnStaticClass)(), const FString &name, const FString &path) {
    QReflection::RegisterClass(fnRegister, fnStaticClass, name);
}

void initStructOnStart(QStruct *(*fnRegister)(), QStruct *(*fnStaticStruct)(), const FString &name, const FString &path) {
    QReflection::RegisterStruct(fnRegister, fnStaticStruct, name);
}

void initEnumOnStart(QEnum *(*fnRegister)(), const FString &name, const FString &path) {
    // TODO: do something?
    (*fnRegister)();
}

class QEnum *getStaticEnum(QEnum *(*fnRegister)(), const FString &name) {
    return (*fnRegister)();
}