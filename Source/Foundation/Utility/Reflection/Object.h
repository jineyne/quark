#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "FileSystem/Stream.h"
#include "Serialization/Archive.h"
#include "Reflection/ReflectionMacros.h"

#define BODY_MACRO_COMBINE_INNER(A,B,C,D) A##B##C##D
#define BODY_MACRO_COMBINE(A,B,C,D) BODY_MACRO_COMBINE_INNER(A,B,C,D)
#define GENERATED_BODY(...) BODY_MACRO_COMBINE(CURRENT_FILE_ID,_,__LINE__,_GENERATED_BODY)

class DLL_EXPORT QObject {
private:
    class QClass *mClass = nullptr;
    FString mName = TEXT("UnInitialized Object");

public:
    QObject() = default;
    QObject(class QClass *myClass, const FString &name);

    virtual ~QObject() = default;

public:
    virtual void serialize(FArchive &archive);

    void rename(const FString &name);

    class QClass *getClass() const { return mClass; }
    const FString &getName() const { return mName; }

    template<typename T>
    bool isA(T base) const {
        const QClass *baseClass = base;
        assert(baseClass && "isA(nullptr) is not available");

        class QClass *thisClass = getClass();
        return thisClass->isChildOf(base);
    }

    template<class T>
    bool isA() const {
        return isA(T::StaticClass());
    }

private:
    void setClass(QClass *newClass);

    // REFLECTION
public:
    friend class QReflection;
    DECLARE_CLASS(QObject, QObject, NO_API);
    static void StaticRegisterNativeQObject() {
    }
};

DLL_EXPORT void initClassOnStart(class QClass *(fnRegister)(), class QClass *(fnStaticClass)(), const FString &name, const FString &path);
struct FInitClassOnStart {
    FInitClassOnStart(class QClass *(fnRegister)(), class QClass *(fnStaticClass)(), const FString &name, const FString &path) {
        initClassOnStart(fnRegister, fnStaticClass, name, path);
    }
};

DLL_EXPORT void initStructOnStart(class QStruct *(fnRegister)(), class QStruct *(fnStaticStruct)(), const FString &name, const FString &path);
struct FInitStructOnStart {
    FInitStructOnStart(class QStruct *(fnRegister)(), class QStruct *(fnStaticStruct)(), const FString &name, const FString &path) {
        initStructOnStart(fnRegister, fnStaticStruct, name, path);
    }
};

DLL_EXPORT void initEnumOnStart(class QEnum *(fnRegister)(), const FString &name, const FString &path);
struct FInitEnumOnStart {
    FInitEnumOnStart(class QEnum *(fnRegister)(), const FString &name, const FString &path) {
        initEnumOnStart(fnRegister, name, path);
    }
};

DLL_EXPORT class QEnum *getStaticEnum(class QEnum *(fnRegister)(), const FString &name);
