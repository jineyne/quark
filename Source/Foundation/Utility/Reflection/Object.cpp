#include "Object.h"

#include "Reflection/Class.h"
#include "Reflection/Enum.h"
#include "Reflection/Reflection.h"
#include "Reflection/ObjectHash.h"

Class *Object::GetPrivateStaticClass() {
    static Class *instance = nullptr;
    if (!instance) {
        // CREATE OBJECT MANUALLY?
        instance = q_new<Class>(sizeof(Object), nullptr);
        instance->mName = TEXT("Object");

        gObjectHash().add(instance);
    }
    return instance;
}

Object::Object(Class *myClass, const String &name)
        : mClass(myClass), mName(name) { }

void Object::serialize(Archive &archive) {
    Class *clazz = getClass();

    if (archive.isSaving()) {
        auto fields = clazz->getCppProperties();

        size_t length = fields.length();
        archive << length;

        for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            archive << name;
            property->serializeElement(this, archive);
        }
    } else {
        size_t length = 0;
        archive << length;

        for (int i = 0; i < length; i++) {
            String name;
            archive << name;
            if (name.empty()) {
                break;
            }

            auto field = clazz->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            property->serializeElement(this, archive);
        }
    }
}

void Object::rename(const String &name) {
    gObjectHash().remove(this);

    mName = name;

    gObjectHash().add(this);
}

void Object::setClass(Class* newClass) {
    gObjectHash().remove(this);

    mClass = newClass;

    gObjectHash().add(this);
}

void Object::setId(size_t id) {
    mId = id;
}

void initClassOnStart(Class *(*fnRegister)(), Class *(*fnStaticClass)(), const String &name, const String &path) {
    Reflection::RegisterClass(fnRegister, fnStaticClass, name);
}

void initStructOnStart(Struct *(*fnRegister)(), Struct *(*fnStaticStruct)(), const String &name, const String &path) {
    Reflection::RegisterStruct(fnRegister, fnStaticStruct, name);
}

void initEnumOnStart(Enum *(*fnRegister)(), const String &name, const String &path) {
    // TODO: do something?
    (*fnRegister)();
}

class Enum *getStaticEnum(Enum *(*fnRegister)(), const String &name) {
    return (*fnRegister)();
}