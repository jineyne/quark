#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Struct.h"

class DLL_EXPORT Class : public Struct {
public:
    typedef void (*ClassConstructorType) (void *);

public:
    ClassConstructorType classConstructor;

    Class *parent = nullptr;
    mutable uint32_t classUnique = 0;

public:
    Class(size_t size, ClassConstructorType fnClassConstructor);

public:
    Class *getSuperClass() const {
        return (Class *)getSuperStruct();
    }

private:
    void initProperties() override;

public:
    DECLARE_CLASS(Class, Struct, );
    static void StaticRegisterNativeClass() {
    }
};


template<class T>
void InternalConstructor(void *data) {
    T::__DefaultConstructor(data);
}