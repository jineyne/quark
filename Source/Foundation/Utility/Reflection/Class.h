#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Struct.h"

class DLL_EXPORT QClass : public QStruct {
public:
    QClass *parent = nullptr;
    mutable uint32_t classUnique = 0;

public:
    QClass(size_t size);

public:
    QClass *getSuperClass() const {
        return (QClass *)getSuperStruct();
    }

public:
    DECLARE_CLASS(QClass, QStruct, );
    static void StaticRegisterNativeQClass() {
    }
};


