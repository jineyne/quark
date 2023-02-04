#include "Object.h"

#include "Reflection/Class.h"
#include "Reflection/Enum.h"
#include "Reflection/Reflection.h"
#include "Reflection/ObjectHash.h"

QClass *QObject::GetPrivateStaticClass() {
    static QClass *instance = nullptr;
    if (!instance) {
        // CREATE OBJECT MANUALLY?
        instance = new QClass(sizeof(QObject), nullptr);
        instance->mName = TEXT("QObject");

        gObjectHash().add(instance);
    }
    return instance;
}

QObject::QObject(QClass *myClass, const FString &name)
        : mClass(myClass), mName(name) { }

void QObject::serialize(FArchive &archive) {
    QClass *clazz = getClass();

    if (archive.isSaving()) {
        auto fields = clazz->getCppProperties();

        size_t length = fields.length();
        archive << length;

        for (auto field : fields) {
            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            FString &name = const_cast<FString &>(property->getName());

            archive << name;
            property->serializeElement(this, archive);
        }
    } else {
        size_t length = 0;
        archive << length;

        for (int i = 0; i < length; i++) {
            FString name;
            archive << name;
            if (name.empty()) {
                break;
            }

            auto field = clazz->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            property->serializeElement(this, archive);
        }
    }
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

void QObject::setId(size_t id) {
    mId = id;
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