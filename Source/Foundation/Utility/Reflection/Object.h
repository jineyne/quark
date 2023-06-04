#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "FileSystem/Stream.h"
#include "Serialization/Archive.h"
#include "Reflection/ReflectionMacros.h"

#define BODY_MACRO_COMBINE_INNER(A,B,C,D) A##B##C##D
#define BODY_MACRO_COMBINE(A,B,C,D) BODY_MACRO_COMBINE_INNER(A,B,C,D)

#if defined(_QHT_GENERATED)
#define GENERATED_BODY(...)
#else
#define GENERATED_BODY(...) BODY_MACRO_COMBINE(CURRENT_FILE_ID,_,__LINE__,_GENERATED_BODY)
#endif

class DLL_EXPORT Object {
private:
    size_t mId;
    class Class *mClass = nullptr;
    String mName = TEXT("UnInitialized Object");

public:
    Object() = default;
    Object(class Class *myClass, const String &name);

    virtual ~Object();

public:
    virtual void serialize(Archive &archive);

    void rename(const String &name);

    const size_t &getId() const { return mId; }
    class Class *getClass() const { return mClass; }
    const String &getName() const { return mName; }

    template<typename T>
    bool isA(T base) const {
        const Class *baseClass = base;
        assert(baseClass && "isA(nullptr) is not available");

        class Class *thisClass = getClass();
        return thisClass->isChildOf(base);
    }

    template<class T>
    bool isA() const {
        return isA(T::StaticClass());
    }

private:
    void setClass(Class *newClass);
    void setId(size_t id);

    // REFLECTION
public:
    friend class Reflection;
    friend class ObjectHash;
    DECLARE_CLASS(Object, Object, NO_API);
    static void StaticRegisterNativeObject() {
    }
};

DLL_EXPORT void initClassOnStart(class Class *(fnRegister)(), class Class *(fnStaticClass)(), const String &name, const String &path);
struct InitClassOnStart {
    InitClassOnStart(class Class *(fnRegister)(), class Class *(fnStaticClass)(), const String &name, const String &path) {
        initClassOnStart(fnRegister, fnStaticClass, name, path);
    }
};

DLL_EXPORT void initStructOnStart(class Struct *(fnRegister)(), class Struct *(fnStaticStruct)(), const String &name, const String &path);
struct FInitStructOnStart {
    FInitStructOnStart(class Struct *(fnRegister)(), class Struct *(fnStaticStruct)(), const String &name, const String &path) {
        initStructOnStart(fnRegister, fnStaticStruct, name, path);
    }
};

DLL_EXPORT void initEnumOnStart(class Enum *(fnRegister)(), const String &name, const String &path);
struct FInitEnumOnStart {
    FInitEnumOnStart(class Enum *(fnRegister)(), const String &name, const String &path) {
        initEnumOnStart(fnRegister, name, path);
    }
};

DLL_EXPORT class Enum *getStaticEnum(class Enum *(fnRegister)(), const String &name);

/**	Hash mValue generator for Object. */
template<>
struct std::hash<Object> {
    size_t operator()(Object *object) const {
        return object->getId();
    }
};