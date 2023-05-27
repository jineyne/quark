#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Reflection/Field.h"

class DLL_EXPORT Struct : public Field {
private:
    TArray<Field *> mChildProperties;
    TArray<Field *> mChildren;

    // TODO: INTERFACE?
    Struct *mSuperStruct = nullptr;

    size_t mSize = 0;

protected:
    bool bIsPropertyInitialized = false;

public:
    Struct(Struct *super, String name, size_t size);

public:
    bool isChildOf(Struct *base) const;

    Struct *getSuperStruct() const { return mSuperStruct; }
    virtual void setSuperStruct(Struct *newStruct);

    void addCppProperty(class Property *property);
    TArray<Field *> getCppProperties(bool deepSearch = true);

    Field *getCppPropertiesByName(const String &name, bool deepSearch = true);

    void setSize(size_t size) { mSize = size; }
    const size_t &getSize() const { return mSize; }

private:
    virtual void initProperties();

public:
    DECLARE_CLASS(Struct, Field, );
    static void StaticRegisterNativeStruct() {
    }
};
