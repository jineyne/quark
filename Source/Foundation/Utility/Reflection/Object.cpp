#include "Object.h"

#include "Reflection/Class.h"
#include "Reflection/Enum.h"
#include "Reflection/Reflection.h"
#include "Reflection/ObjectHash.h"

#if REFLECTION_ENABLE
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
#endif

Object::Object(Class *myClass, const String &name)
        : mClass(myClass), mName(name) { }

Object::~Object() {
    gObjectHash().remove(this);
}

void Object::serialize(ArchiveFormatter &formatter) {
    Class *clazz = getClass();
    formatter.enterField(*const_cast<String*>( &clazz->getName()));
    formatter.enterRecord();

    if (formatter.isSaving()) {
        auto fields = clazz->getCppProperties();

        for (auto field : fields) {
            if (!field->isA<Property>()) {
                continue;
            }

            auto property = (Property *) field;
            String &name = const_cast<String &>(property->getName());

            formatter.enterField(name);
            property->serializeElement(this, formatter);
            formatter.leaveField();
        }
    } else {
        // isLoading
        // TODO: class property 정보를 미리 저장해서 직렬화를 부드럽게 진행행할것

        String fieldName = String::Empty;
        while ((formatter.enterField(fieldName), !fieldName.empty())) {
            auto field = clazz->getCppPropertiesByName(fieldName);
            if (!field) {
                // How to undo name
                formatter.leaveField();
                break;
            }

            if (!field->isA<Property>()) {
                formatter.leaveField();
                break;
            }

            auto property = (Property *) field;
            property->serializeElement(this, formatter);
            formatter.leaveField();
        }
    }

    formatter.leaveRecord();
    formatter.leaveField();
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