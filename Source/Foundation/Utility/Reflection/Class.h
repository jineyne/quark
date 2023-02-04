#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Struct.h"

class DLL_EXPORT QClass : public QStruct {
public:
    typedef void (*ClassConstructorType) (void *);

public:
    ClassConstructorType classConstructor;

    QClass *parent = nullptr;
    mutable uint32_t classUnique = 0;

public:
    QClass(size_t size, ClassConstructorType fnClassConstructor);

public:
    QClass *getSuperClass() const {
        return (QClass *)getSuperStruct();
    }

private:
    void initProperties() override;

public:
    DECLARE_CLASS(QClass, QStruct, );
    static void StaticRegisterNativeQClass() {
    }
};


template<class T>
void InternalConstructor(void *data) {
    T::__DefaultConstructor(data);
}